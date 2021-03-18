//
// Created by pc on 19-11-20.
//

#include "AstNodeNumber.h"

#include <cassert>

AstNodeNumber::AstNodeNumber(const std::string& string) : m_value(string) {
}

AstNodeNumber::AstNodeNumber(long long int val) : m_value(val) {
}

AstNodeNumber::AstNodeNumber(const Number& number) : m_value(number) {
}

std::string AstNodeNumber::toString() const {
    return m_value.toString();
}

u_ptr_AstNode AstNodeNumber::copy() const {
    return u_ptr_AstNode(new AstNodeNumber(m_value));
}

u_ptr_AstNode AstNodeNumber::simplify(SIMPLIFY_RULES simplifyRules) const {
    return copy();
}

AstNode::NODE_TYPE AstNodeNumber::type() const {
    return NODE_TYPE::NUMBER;
}

bool AstNodeNumber::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::NUMBER) {
        return m_value == dynamic_cast<const AstNodeNumber&>(other).m_value;
    }
    return false;
}

size_t AstNodeNumber::childCount() const {
    return 0;
}

const AstNode* AstNodeNumber::childAt(size_t index) const {
    assert(false);
}

bool AstNodeNumber::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    const auto* cast = dynamic_cast<const AstNodeNumber*>(rhs);
    if (m_value.number().index() != cast->m_value.number().index()) {
        return m_value.number().index() < cast->m_value.number().index();
    }
    return m_value < cast->m_value;
}

const Number& AstNodeNumber::value() const {
    return m_value;
}
