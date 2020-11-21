//
// Created by pc on 19-11-20.
//

#include "AstNodeSubtract.h"

AstNodeSubtract::AstNodeSubtract(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeSubtract::toString() const {
    return "(" + m_leftNode->toString() + " - " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeSubtract::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeSubtract(m_leftNode->copy(), m_rightNode->copy()));
}
std::unique_ptr<AstNode> AstNodeSubtract::simplify() const {
    AstNode* simplifiedNode = new AstNodeSubtract(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeSubtract::type() const {
    return NODE_TYPE::SUBTRACT;
}
