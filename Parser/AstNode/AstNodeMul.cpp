//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

#include <cassert>

AstNodeMul::AstNodeMul()
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs * rhs; },
                         [](const u_ptr_AstNode& node) { return node->isOne(); }) {
}

AstNodeMul::AstNodeMul(u_ptr_AstNode&& left, u_ptr_AstNode&& right)
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs * rhs; },
                         [](u_ptr_AstNode& node) { return node->isOne(); }) {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));

    cleanUp();
}

AstNodeMul::AstNodeMul(std::vector<u_ptr_AstNode>&& nodes)
    : AstNodeCommutative(
          std::move(nodes), [](const Numeric& lhs, const Numeric& rhs) { return lhs * rhs; },
          [](const u_ptr_AstNode& node) { return node->isOne(); }) {
    if (m_nodes.empty()) {
        m_nodes.emplace_back(AstNode::one());
    }
    cleanUp();
}

std::string AstNodeMul::toString() const {
    std::string result = "(";
    for (const auto& it : m_nodes) {
        result += it->toString() + " * ";
    }
    result = result.substr(0, result.length() - 3ul);
    result += ")";
    return result;
}

u_ptr_AstNode AstNodeMul::copy() const {
    auto copy = u_ptr_AstNode(new AstNodeMul{});
    for (const auto& it : m_nodes) {
        COMMUTATIVE_CAST(copy.get())->addNode(it->copy());
    }
    return copy;
}

u_ptr_AstNode AstNodeMul::simplify() const {
    u_ptr_AstNode simplifiedNode = simplifiedCopy();
    auto*         node           = dynamic_cast<AstNodeMul*>(simplifiedNode.get());
    node->cleanUp();

    if (node->childCount() == 0) {
        return AstNode::one();
    } else if (node->childCount() == 1) {
        return std::move(node->m_nodes[0]);
    }

    if (node->gatherDuplicates()) {
        return node->simplify();
    }

    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeMul::type() const {
    return NODE_TYPE::MULTIPLY;
}

u_ptr_AstNode AstNodeMul::simplifiedCopy() const {
    auto* result = new AstNodeMul{};
    for (const auto& it : m_nodes) {
        result->addNode(it->simplify());
    }

    return u_ptr_AstNode(result);
}

bool AstNodeMul::gatherDuplicates() {
    assert(std::is_sorted(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr));
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        const auto count = countCopies(it->get());
        if (count > 1ul) {
            auto copy = (*it)->copy();
            m_nodes.erase(it + 1, it + count);
            *it = std::move(*it) ^ AstNode::makeInteger(static_cast<long long>(count));
            return true;
        }
    }
    return false;
}
