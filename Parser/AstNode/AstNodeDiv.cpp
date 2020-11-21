//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

#include <cassert>

AstNodeDiv::AstNodeDiv(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent)
    : m_leftNode(std::move(base)), m_rightNode(std::move(exponent)) {
}

std::string AstNodeDiv::toString() const {
    return "(" + m_leftNode->toString() + " / " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeDiv::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeDiv(m_leftNode->copy(), m_rightNode->copy()));
}

std::unique_ptr<AstNode> AstNodeDiv::simplify() const {
    const auto left  = m_leftNode->simplify();
    const auto right = m_rightNode->simplify();
    if (left->isNumeric() && right->isNumeric()) {
        return doBinaryOperation(left, right, std::divides<>(), std::divides<>());
    }

    AstNode* simplifiedNode = new AstNodeDiv(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeDiv::type() const {
    return NODE_TYPE::DIVIDE;
}

bool AstNodeDiv::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::DIVIDE) {
        const auto& candidate = dynamic_cast<const AstNodeDiv&>(other);
        return (*m_leftNode == *candidate.m_leftNode && *m_rightNode == *candidate.m_rightNode);
    }
    return false;
}

size_t AstNodeDiv::childCount() const {
    return 2ul;
}

const AstNode* AstNodeDiv::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}
