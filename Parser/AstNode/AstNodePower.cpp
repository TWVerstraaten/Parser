//
// Created by pc on 19-11-20.
//

#include "AstNodePower.h"

#include "AstNodeNumber.h"

#include <cassert>
#include <cmath>

AstNodePower::AstNodePower(u_ptr_AstNode&& base, u_ptr_AstNode&& exponent) : m_base(std::move(base)), m_exponent(std::move(exponent)) {
}

std::string AstNodePower::toString() const {
    return "(" + m_base->toString() + " ^ " + m_exponent->toString() + ")";
}

u_ptr_AstNode AstNodePower::copy() const {
    return u_ptr_AstNode(new AstNodePower(m_base->copy(), m_exponent->copy()));
}

u_ptr_AstNode AstNodePower::simplify() const {
    auto       base     = m_base->simplify();
    const auto exponent = m_exponent->simplify();
    if (exponent->isOne()) {
        return base;
    } else if (exponent->isZero()) {
        return AstNode::makeOneNode();
    }
    if (m_base->type() == AstNode::NODE_TYPE::POWER) {
        return std::make_unique<AstNodePower>(
            m_base->childAt(0)->copy(), (*m_base->childAt(1) * *m_exponent)->simplify());
    }
    if (m_base->type() == AstNode::NODE_TYPE::UNARY_MINUS && m_exponent->isEven()) {
        return std::make_unique<AstNodePower>(m_base->childAt(0)->copy(), m_exponent->copy())->simplify();
    }
    AstNode* simplifiedNode =
        new AstNodePower(m_base->simplify(), m_exponent->simplify());
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodePower::type() const {
    return NODE_TYPE::POWER;
}

bool AstNodePower::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::POWER) {
        return (*m_base == *other.childAt(0) && *m_exponent == *other.childAt(1));
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
    if (*m_base == *rhs->childAt(0)) {
        return compare(m_exponent.get(), rhs->childAt(1));
    } else {
        return compare(m_base.get(), rhs->childAt(0));
    }
}

u_ptr_AstNode AstNodePower::differentiate(const std::string& variable) const {
    return makeError();
}

std::set<std::string> AstNodePower::collectVariables() const {
    auto result = m_base->collectVariables();
    result.merge(m_exponent->collectVariables());
    return result;
}
