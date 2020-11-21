//
// Created by pc on 19-11-20.
//

#include "AstNodePower.h"

AstNodePower::AstNodePower(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent)
    : m_base(std::move(base)), m_exponent(std::move(exponent)) {
}

std::string AstNodePower::toString() const {
    return "(" + m_base->toString() + " ^ " + m_exponent->toString() + ")";
}

std::unique_ptr<AstNode> AstNodePower::copy() const {
    return std::unique_ptr<AstNode>(new AstNodePower(m_base->copy(), m_exponent->copy()));
}
std::unique_ptr<AstNode> AstNodePower::simplify() const {
    AstNode* simplifiedNode = new AstNodePower(m_base->simplify(), m_exponent->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}
AstNode::NODE_TYPE AstNodePower::type() const {
    return NODE_TYPE::POWER;
}
