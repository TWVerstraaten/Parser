//
// Created by pc on 19-11-20.
//

#include "AstNodePower.h"

AstNodePower::AstNodePower(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent)
    : AstNode(NODE_TYPE::POW), m_base(std::move(base)), m_exponent(std::move(exponent)) {
}

std::string AstNodePower::toString() const {
    return "(" + m_base->toString() + " ^ " + m_exponent->toString() + ")";
}
