//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

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
    AstNode* simplifiedNode = new AstNodeMul(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeMul::type() const {
    return NODE_TYPE::MUL;
}
