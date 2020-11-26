//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

#include <cassert>
#include <iostream>

AstNodeUnaryMinus::AstNodeUnaryMinus(u_ptr_AstNode&& value) : m_value(std::move(value)) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}

u_ptr_AstNode AstNodeUnaryMinus::copy() const {
    return u_ptr_AstNode(new AstNodeUnaryMinus(m_value->copy()));
}

u_ptr_AstNode AstNodeUnaryMinus::simplify() const {
    AstNode* simplifiedNode = new AstNodeUnaryMinus(m_value->simplify());
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeUnaryMinus::type() const {
    return NODE_TYPE::UNARY_MINUS;
}

bool AstNodeUnaryMinus::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::UNARY_MINUS) {
        return *m_value == *dynamic_cast<const AstNodeUnaryMinus&>(other).m_value;
    }
    return false;
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
    return compare_u_ptr(m_value, dynamic_cast<const AstNodeUnaryMinus*>(rhs)->m_value);
}
