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
    return other.type() == AstNode::NODE_TYPE::UNARY_MINUS && *m_value == *other.childAt(0);
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
