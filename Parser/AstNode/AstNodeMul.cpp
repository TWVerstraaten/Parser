//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

AstNodeMul::AstNodeMul(AstNode* base, AstNode* exponent) : AstNode(NODE_TYPE::MUL), m_left(base), m_right(exponent) {
}

std::string AstNodeMul::toString() const {
    return "(" + m_left->toString() + " * " + m_right->toString() + ")";
}
