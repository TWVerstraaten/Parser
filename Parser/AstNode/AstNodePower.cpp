//
// Created by pc on 19-11-20.
//

#include "AstNodePower.h"

#include <cassert>
#include <cmath>

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
    const auto base     = m_base->simplify();
    const auto exponent = m_exponent->simplify();
    if (base->isNumeric() && exponent->isNumeric()) {
        return doBinaryOperation(
            base, exponent, [](double a, double b) { return std::pow(a, b); },
            [](double a, double b) { return std::pow(a, b); });
    }

    AstNode* simplifiedNode = new AstNodePower(m_base->simplify(), m_exponent->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodePower::type() const {
    return NODE_TYPE::POWER;
}

bool AstNodePower::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::POWER) {
        const auto& candidate = dynamic_cast<const AstNodePower&>(other);
        return (*m_base == *candidate.m_base && *m_exponent == *candidate.m_exponent);
    }
    return false;
}
size_t AstNodePower::childCount() const {
    return 2;
}
const AstNode* AstNodePower::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_base.get() : m_exponent.get();
}
