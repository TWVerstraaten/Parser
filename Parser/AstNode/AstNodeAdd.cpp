//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeInteger.h"
#include "AstNodeUnaryMinus.h"

#include <iostream>
#include <memory>

AstNodeAdd::AstNodeAdd()
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
                         [](const u_ptr_AstNode& node) { return node->isZero(); }) {
}

AstNodeAdd::AstNodeAdd(u_ptr_AstNode&& left, u_ptr_AstNode&& right)
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
                         [](u_ptr_AstNode& node) { return node->isZero(); }) {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));

    cleanUp();
}

std::string AstNodeAdd::toString() const {
    std::string result = "(";
    for (const auto& it : m_nodes) {
        result += it->toString() + " + ";
    }
    result = result.substr(0, result.length() - 3ul);
    result += ")";
    return result;
}

u_ptr_AstNode AstNodeAdd::copy() const {
    auto copy = u_ptr_AstNode(new AstNodeAdd{});
    for (const auto& it : m_nodes) {
        dynamic_cast<AstNodeCommutative*>(copy.get())->addNode(it->copy());
    }
    return copy;
}

u_ptr_AstNode AstNodeAdd::simplify() const {
    u_ptr_AstNode simplifiedNode = simplifiedCopy();
    auto*         node           = dynamic_cast<AstNodeAdd*>(simplifiedNode.get());
    node->cleanUp();
    node->removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE::UNARY_MINUS);

    if (node->childCount() == 0) {
        return u_ptr_AstNode(new AstNodeInteger(0));
    } else if (node->childCount() == 0) {
        return std::move(node->m_nodes[0]);
    }
    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}
u_ptr_AstNode AstNodeAdd::simplifiedCopy() const {
    auto* result = new AstNodeAdd{};
    for (const auto& it : m_nodes) {
        result->addNode(it->simplify());
    }
    return u_ptr_AstNode(result);
}
