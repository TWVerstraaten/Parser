//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

AstNodeAdd::AstNodeAdd(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : AstNode(NODE_TYPE::ADD), m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeAdd::toString() const {
    return "(" + m_leftNode->toString() + " + " + m_rightNode->toString() + ")";
}
