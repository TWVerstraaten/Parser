//
// Created by pc on 25-11-20.
//

#include "AstNodeCommutative.h"

#include "AstNodeNumeric.h"

#include <cassert>

AstNodeCommutative::AstNodeCommutative(std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                                       std::function<bool(u_ptr_AstNode&)>                    removePredicate)
    : m_accumulator(std::move(accumulator)), m_removePredicate(std::move(removePredicate)) {
}

AstNodeCommutative::AstNodeCommutative(std::vector<u_ptr_AstNode>&&                           nodes,
                                       std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                                       std::function<bool(u_ptr_AstNode&)>                    removePredicate)
    : m_accumulator(std::move(accumulator)), m_removePredicate(std::move(removePredicate)), m_nodes(std::move(nodes)) {
}

void AstNodeCommutative::removeNode(const AstNode* nodeToRemove) {
    assert(std::find_if(m_nodes.begin(), m_nodes.end(),
                        [&](const u_ptr_AstNode& node) { return *node.get() == *nodeToRemove; }) != m_nodes.end());
    m_nodes.erase(std::find_if(m_nodes.begin(), m_nodes.end(),
                               [&](const u_ptr_AstNode& node) { return *node.get() == *nodeToRemove; }));
}

void AstNodeCommutative::removeAllCopiesOfNode(const AstNode* nodeToRemove) {
    while (std::find_if(m_nodes.begin(), m_nodes.end(),
                        [&](const u_ptr_AstNode& node) { return *node.get() == *nodeToRemove; }) != m_nodes.end()) {
        removeNode(nodeToRemove);
        if (m_nodes.empty()) {
            return;
        }
    }
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
            auto* childNode = COMMUTATIVE_CAST(it->release());
            it              = m_nodes.erase(it);
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
    if (other.childCount() != childCount()) {
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
    const auto& rightNodes = COMMUTATIVE_C_CAST(rhs)->m_nodes;
    return std::lexicographical_compare(m_nodes.begin(), m_nodes.end(), rightNodes.begin(), rightNodes.end(),
                                        AstNode::compare_u_ptr);
}

std::pair<const AstNode*, const AstNode*> AstNodeCommutative::findViaTypeContainingCopy(AstNode::NODE_TYPE type) const {
    for (size_t i = 0; i != childCount(); ++i) {
        for (size_t j = 0; j != childCount(); ++j) {
            if (i != j && childAt(j)->type() == type && childAt(j)->containsCopyOf(childAt(i))) {
                return {childAt(i), childAt(j)};
            }
        }
    }
    return {nullptr, nullptr};
}

void AstNodeCommutative::removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE type) {
    std::pair<const AstNode*, const AstNode*> pair = findViaTypeContainingCopy(type);
    while (pair.first != nullptr) {
        assert(pair.second != nullptr);
        removeNode(pair.first);
        removeNode(pair.second);
        pair = findViaTypeContainingCopy(AstNode::NODE_TYPE::UNARY_MINUS);
    }
}

Decomposition AstNodeCommutative::decompose(const AstNodeCommutative* A, const AstNodeCommutative* B) {
    assert(A != B);
    std::vector<const AstNode*> aCopy;
    std::vector<const AstNode*> bCopy;
    std::transform(A->m_nodes.begin(), A->m_nodes.end(), std::back_inserter(aCopy),
                   [](const u_ptr_AstNode& u_ptr) { return u_ptr.get(); });
    std::transform(B->m_nodes.begin(), B->m_nodes.end(), std::back_inserter(bCopy),
                   [](const u_ptr_AstNode& u_ptr) { return u_ptr.get(); });
    std::sort(aCopy.begin(), aCopy.end(), AstNode::compare);
    std::sort(bCopy.begin(), bCopy.end(), AstNode::compare);

    return alg::decomposeSorted(aCopy, bCopy, [](const AstNode* node) { return node->copy(); });
}

AstNode& AstNodeCommutative::operator[](size_t index) {
    return *m_nodes[index];
}

std::vector<u_ptr_AstNode>::const_iterator AstNodeCommutative::mulBegin() const {
    return std::find_if(m_nodes.begin(), m_nodes.end(),
                        [](const auto& node) { return node->type() == AstNode::NODE_TYPE::MULTIPLY; });
}

std::vector<u_ptr_AstNode>::const_iterator AstNodeCommutative::mulEnd() const {
    auto it = mulBegin();
    while (it != m_nodes.end() && (*it)->type() == AstNode::NODE_TYPE::MULTIPLY) {
        ++it;
    }
    return it;
}

std::vector<u_ptr_AstNode>::const_iterator AstNodeCommutative::addBegin() const {
    return std::find_if(m_nodes.begin(), m_nodes.end(),
                        [](const auto& node) { return node->type() == AstNode::NODE_TYPE::ADD; });
}

std::vector<u_ptr_AstNode>::const_iterator AstNodeCommutative::addEnd() const {
    auto it = mulBegin();
    while (it != m_nodes.end() && (*it)->type() == AstNode::NODE_TYPE::ADD) {
        ++it;
    }
    return it;
}

size_t AstNodeCommutative::countCopies(const AstNode* nodeToCompare) const {
    return std::count_if(m_nodes.begin(), m_nodes.end(), [&](const auto& node) { return *node == *nodeToCompare; });
}

void AstNodeCommutative::removeNullptrs() {
    m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(), [](const auto& node) { return node == nullptr; }),
                  end(m_nodes));
}
