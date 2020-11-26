//
// Created by pc on 19-11-20.
//

#include "AstNodePower.h"

#include "AstNodeNumeric.h"

#include <cassert>
#include <cmath>

AstNodePower::AstNodePower(u_ptr_AstNode&& base, u_ptr_AstNode&& exponent)
    : m_base(std::move(base)), m_exponent(std::move(exponent)) {
}

std::string AstNodePower::toString() const {
    return "(" + m_base->toString() + " ^ " + m_exponent->toString() + ")";
}

u_ptr_AstNode AstNodePower::copy() const {
    return u_ptr_AstNode(new AstNodePower(m_base->copy(), m_exponent->copy()));
}

u_ptr_AstNode AstNodePower::simplify() const {
    const auto base     = m_base->simplify();
    const auto exponent = m_exponent->simplify();
    if (base->isNumeric() && exponent->isNumeric()) {
        return (NUMERIC_CAST(base.get()) ^ NUMERIC_CAST(exponent.get())).toNode();
    }

    AstNode* simplifiedNode = new AstNodePower(m_base->simplify(), m_exponent->simplify());
    return u_ptr_AstNode(simplifiedNode);
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

bool AstNodePower::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    const auto* rightSide = dynamic_cast<const AstNodePower*>(rhs);
    if (*m_base == *rightSide->m_base) {
        return compare_u_ptr(m_exponent, rightSide->m_exponent);
    } else {
        return compare_u_ptr(m_base, rightSide->m_base);
    }
}
