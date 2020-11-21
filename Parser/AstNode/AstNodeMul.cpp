//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"

AstNodeMul::AstNodeMul(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeMul::toString() const {
    return "(" + m_leftNode->toString() + " * " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeMul::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeMul(m_leftNode->copy(), m_rightNode->copy()));
}

std::unique_ptr<AstNode> AstNodeMul::simplify() const {
    const auto left  = m_leftNode->simplify();
    const auto right = m_rightNode->simplify();
    if (left->isNumeric() && right->isNumeric()) {
        return doBinaryOperation(left, right, std::multiplies<>(), std::multiplies<>());
    }

    AstNode* simplifiedNode = new AstNodeMul(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeMul::type() const {
    return NODE_TYPE::MULTIPLY;
}

bool AstNodeMul::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::MULTIPLY) {
        const auto& candidate = dynamic_cast<const AstNodeMul&>(other);
        return (*m_leftNode == *candidate.m_leftNode && *m_rightNode == *candidate.m_rightNode) ||
               (*m_rightNode == *candidate.m_leftNode && *m_leftNode == *candidate.m_rightNode);
    }
    return false;
}
