//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

AstNodeDiv::AstNodeDiv(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent)
    : AstNode(NODE_TYPE::DIV), m_left(std::move(base)), m_right(std::move(exponent)) {
}

std::string AstNodeDiv::toString() const {
    return "(" + m_left->toString() + " / " + m_right->toString() + ")";
}
