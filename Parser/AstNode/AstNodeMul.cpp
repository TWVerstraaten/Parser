//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

#include "AstNodeInteger.h"

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
        return u_ptr_AstNode(new AstNodeInteger(1));
    } else if (node->childCount() == 1) {
        return std::move(node->m_nodes[0]);
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
