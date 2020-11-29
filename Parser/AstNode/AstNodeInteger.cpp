//
// Created by pc on 19-11-20.
//

#include "AstNodeInteger.h"

#include <cassert>

AstNodeInteger::AstNodeInteger(const std::string& string) : m_value(stoll(string)) {
}

AstNodeInteger::AstNodeInteger(long long int val) : m_value(val) {
}

std::string AstNodeInteger::toString() const {
    return std::to_string(m_value);
}

u_ptr_AstNode AstNodeInteger::copy() const {
    return u_ptr_AstNode(new AstNodeInteger(m_value));
}

u_ptr_AstNode AstNodeInteger::simplify() const {
    return copy();
}

AstNode::NODE_TYPE AstNodeInteger::type() const {
    return NODE_TYPE::INTEGER;
}

bool AstNodeInteger::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::INTEGER) {
        return m_value == dynamic_cast<const AstNodeInteger&>(other).m_value;
    }
    return false;
}

size_t AstNodeInteger::childCount() const {
    return 0;
}

const AstNode* AstNodeInteger::childAt(size_t index) const {
    assert(false);
}

Numeric AstNodeInteger::toNumeric() const {
    return Numeric(m_value);
}

bool AstNodeInteger::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    return m_value < dynamic_cast<const AstNodeInteger*>(rhs)->m_value;
}
