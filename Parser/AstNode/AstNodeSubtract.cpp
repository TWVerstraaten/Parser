//
// Created by pc on 19-11-20.
//

#include "AstNodeSubtract.h"

AstNodeSubtract::AstNodeSubtract(AstNode* left, AstNode* right)
    : AstNode(NODE_TYPE::SUB), m_leftNode(left), m_rightNode(right) {
}

std::string AstNodeSubtract::toString() const {
    return "(" + m_leftNode->toString() + " - " + m_rightNode->toString() + ")";
}
