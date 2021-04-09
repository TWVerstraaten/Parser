//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

#include "AstNodeAdd.h"
#include "AstNodeNumber.h"

#include <cassert>

AstNodeUnaryMinus::AstNodeUnaryMinus(u_ptr_AstNode&& value) : m_value(std::move(value)) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}

u_ptr_AstNode AstNodeUnaryMinus::copy() const {
    return u_ptr_AstNode(new AstNodeUnaryMinus(m_value->copy()));
}

u_ptr_AstNode AstNodeUnaryMinus::simplify() const {
    if (m_value->type() == AstNode::NODE_TYPE::UNARY_MINUS) {
        return m_value->childAt(0)->simplify();
    }
    if (m_value->isNumeric()) {
        return makeNumber(m_value->getNumber().negate());
    }
    auto* simplifiedNode = new AstNodeUnaryMinus(m_value->simplify());
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeUnaryMinus::type() const {
    return NODE_TYPE::UNARY_MINUS;
}

bool AstNodeUnaryMinus::equals(const AstNode& other) const {
    if (other.type() != AstNode::NODE_TYPE::UNARY_MINUS) {
        return false;
    }
    return m_value == nullptr ? other.childAt(0) == nullptr : *m_value == *other.childAt(0);
}

size_t AstNodeUnaryMinus::childCount() const {
    return 1;
}

const AstNode* AstNodeUnaryMinus::childAt(size_t index) const {
    assert(index < childCount());
    return m_value.get();
}

bool AstNodeUnaryMinus::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    return compare(m_value.get(), rhs->childAt(0));
}
u_ptr_AstNode AstNodeUnaryMinus::differentiate(const std::string& variable) const {
    return -m_value->differentiate(variable);
}

std::set<std::string> AstNodeUnaryMinus::collectVariables() const {
    return m_value->collectVariables();
}
