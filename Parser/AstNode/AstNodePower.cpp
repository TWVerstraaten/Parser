//
// Created by pc on 19-11-20.
//

#include "AstNodePower.h"

AstNodePower::AstNodePower(AstNode* base, AstNode* exponent)
    : AstNode(NODE_TYPE::POW), m_base(base), m_exponent(exponent) {
}

std::string AstNodePower::toString() const {
    return "(" + m_base->toString() + " ^ " + m_exponent->toString() + ")";
}
