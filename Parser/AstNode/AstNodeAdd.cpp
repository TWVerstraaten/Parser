//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

AstNodeAdd::AstNodeAdd(AstNode* left, AstNode* right)
    : AstNode(NODE_TYPE::ADD), m_leftNode(left), m_rightNode(right) {
}

std::string AstNodeAdd::toString() const {
    return "(" + m_leftNode->toString() + " + " + m_rightNode->toString() + ")";
}
