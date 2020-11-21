//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

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
    AstNode* simplifiedNode = new AstNodeDiv(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeDiv::type() const {
    return NODE_TYPE::DIV;
}
