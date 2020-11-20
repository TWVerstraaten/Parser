//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

AstNodeDiv::AstNodeDiv(AstNode* base, AstNode* exponent)
    : AstNode(NODE_TYPE::DIV), m_left(base), m_right(exponent) {
}

std::string AstNodeDiv::toString() const {
    return "(" + m_left->toString() + " / " + m_right->toString() + ")";
}
