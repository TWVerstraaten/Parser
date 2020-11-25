//
// Created by pc on 25-11-20.
//

#include "AstNodeCommutative.h"

#include "AstNodeNumeric.h"

#include <cassert>

void AstNodeCommutative::removeChild(const AstNode* nodeToRemove) {
    assert(std::find_if(m_nodes.begin(), m_nodes.end(), [&](const std::unique_ptr<AstNode>& node) {
               return node.get() == nodeToRemove;
           }) != m_nodes.end());
    m_nodes.erase(std::find_if(m_nodes.begin(), m_nodes.end(),
                               [&](const std::unique_ptr<AstNode>& node) { return node.get() == nodeToRemove; }));
}

size_t AstNodeCommutative::childCount() const {
    return m_nodes.size();
}

const AstNode* AstNodeCommutative::childAt(size_t index) const {
    assert(index < childCount());
    return m_nodes.at(index).get();
}

void AstNodeCommutative::addNode(std::unique_ptr<AstNode> node) {
    m_nodes.emplace_back(std::move(node));
}

void AstNodeCommutative::sortNodes() {
    std::sort(m_nodes.begin(), m_nodes.end(), AstNode::compare);
}

void AstNodeCommutative::mergeNodes() {
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        if ((*it)->type() == type()) {
            auto* childNode = dynamic_cast<AstNodeCommutative*>(it->release());
            m_nodes.erase(it);
            std::move(childNode->m_nodes.begin(), childNode->m_nodes.end(), std::back_inserter(m_nodes));
            mergeNodes();
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

void AstNodeCommutative::accumulateNumeric(const std::function<Numeric(const Numeric&, const Numeric&)>& f) {
    if (m_nodes.size() < 2) {
        return;
    }
    auto it = std::next(m_nodes.begin());
    while (it != m_nodes.end() && (*it)->isNumeric()) {
        std::unique_ptr<const AstNodeNumeric> firstNumber{dynamic_cast<AstNodeNumeric*>((it - 1)->release())};
        std::unique_ptr<const AstNodeNumeric> secondNumber{dynamic_cast<AstNodeNumeric*>((it)->release())};
        it  = m_nodes.erase(it - 1);
        *it = f(firstNumber->toNumeric(), secondNumber->toNumeric()).toNode();
        ++it;
    }
}

void AstNodeCommutative::cleanUp(const std::function<Numeric(const Numeric&, const Numeric&)>& accumulator,
                                 const std::function<bool(const AstNode*)>&                    removePredicate) {
    mergeNodes();
    sortNodes();
    removeIf(removePredicate);
    accumulateNumeric(accumulator);
}
