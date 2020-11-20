//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

AstNodeMul::AstNodeMul(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : AstNode(NODE_TYPE::MUL), m_left(std::move(left)), m_right(std::move(right)) {
}

std::string AstNodeMul::toString() const {
    return "(" + m_left->toString() + " * " + m_right->toString() + ")";
}
