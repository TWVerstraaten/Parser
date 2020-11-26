//
// Created by pc on 25-11-20.
//

#include "AstNodeCommutative.h"

#include "AstNodeNumeric.h"

#include <cassert>
#include <utility>

AstNodeCommutative::AstNodeCommutative(std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                                       std::function<bool(u_ptr_AstNode&)>                    removePredicate)
    : m_accumulator(std::move(accumulator)), m_removePredicate(std::move(removePredicate)) {
}

void AstNodeCommutative::removeChild(const AstNode* nodeToRemove) {
    assert(std::find_if(m_nodes.begin(), m_nodes.end(),
                        [&](const u_ptr_AstNode& node) { return node.get() == nodeToRemove; }) != m_nodes.end());
    m_nodes.erase(std::find_if(m_nodes.begin(), m_nodes.end(),
                               [&](const u_ptr_AstNode& node) { return node.get() == nodeToRemove; }));
}

size_t AstNodeCommutative::childCount() const {
    return m_nodes.size();
}

const AstNode* AstNodeCommutative::childAt(size_t index) const {
    assert(index < childCount());
    return m_nodes.at(index).get();
}

void AstNodeCommutative::addNode(u_ptr_AstNode node) {
    m_nodes.emplace_back(std::move(node));
}

void AstNodeCommutative::sortNodes() {
    std::sort(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr);
}

void AstNodeCommutative::mergeNodes() {
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        if ((*it)->type() == type()) {
            auto* childNode = dynamic_cast<AstNodeCommutative*>(it->release());
            it              = m_nodes.erase(it);
            std::move(childNode->m_nodes.begin(), childNode->m_nodes.end(), std::back_inserter(m_nodes));
            //            mergeNodes();
            return;
        }
    }
}

bool AstNodeCommutative::equals(const AstNode& other) const {
    if (other.type() != type()) {
        return false;
    }
    const auto& candidate = dynamic_cast<const AstNodeCommutative&>(other);
    if (candidate.childCount() != childCount()) {
        return false;
    }
    for (size_t i = 0; i != childCount(); ++i) {
        if (not(*m_nodes.at(i) == *other.childAt(i))) {
            return false;
        }
    }
    return true;
}

void AstNodeCommutative::accumulateNumeric() {
    if (m_nodes.size() < 2) {
        return;
    }
    auto it = std::next(m_nodes.begin());
    while (it != m_nodes.end() && (*it)->isNumeric()) {
        const Numeric num1 = NUMERIC_CAST((it - 1)->get());
        const Numeric num2 = NUMERIC_CAST(it->get());
        it                 = m_nodes.erase(it - 1);
        *it                = m_accumulator(num1, num2).toNode();
        ++it;
    }
}
void AstNodeCommutative::cleanUp() {
    mergeNodes();
    sortNodes();
    m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(), m_removePredicate), m_nodes.end());
    accumulateNumeric();
    m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(), m_removePredicate), m_nodes.end());
}

bool AstNodeCommutative::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == AstNode::NODE_TYPE::ADD || rhs->type() == AstNode::NODE_TYPE::MULTIPLY);
    const auto& rightNodes = dynamic_cast<const AstNodeCommutative*>(rhs)->m_nodes;
    return std::lexicographical_compare(m_nodes.begin(), m_nodes.end(), rightNodes.begin(), rightNodes.end(),
                                        AstNode::compare_u_ptr);
}

std::pair<const AstNode*, const AstNode*> AstNodeCommutative::findViaTypeContainingCopy(AstNode::NODE_TYPE type) const {
    for (size_t i = 0; i != childCount(); ++i) {
        for (size_t j = 0; j != childCount(); ++j) {
            if (i == j) {
                continue;
            }
            if (childAt(j)->type() == type && childAt(j)->containsCopyOf(childAt(i))) {
                return {childAt(i), childAt(j)};
            }
        }
    }
    return {nullptr, nullptr};
}

void AstNodeCommutative::removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE type) {
    std::pair<const AstNode*, const AstNode*> pair = findViaTypeContainingCopy(type);
    while (pair.first != nullptr) {
        removeChild(pair.first);
        removeChild(pair.second);
        pair = findViaTypeContainingCopy(AstNode::NODE_TYPE::UNARY_MINUS);
    }
}
