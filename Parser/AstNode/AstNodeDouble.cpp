//
// Created by pc on 19-11-20.
//

#include "AstNodeDouble.h"

#include <cassert>

AstNodeDouble::AstNodeDouble(const std::string& string) : m_value(stod(string)) {
}

AstNodeDouble::AstNodeDouble(double value) : m_value(value) {
}

std::string AstNodeDouble::toString() const {
    return std::to_string(m_value);
}

u_ptr_AstNode AstNodeDouble::copy() const {
    return u_ptr_AstNode(new AstNodeDouble(m_value));
}

u_ptr_AstNode AstNodeDouble::simplify() const {
    return copy();
}

AstNode::NODE_TYPE AstNodeDouble::type() const {
    return NODE_TYPE::DOUBLE;
}

double AstNodeDouble::value() const {
    return m_value;
}

bool AstNodeDouble::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::DOUBLE) {
        return m_value == dynamic_cast<const AstNodeDouble&>(other).m_value;
    }
    return false;
}

size_t AstNodeDouble::childCount() const {
    return 0;
}
const AstNode* AstNodeDouble::childAt(size_t index) const {
    assert(false);
    return nullptr;
}

Numeric AstNodeDouble::toNumeric() const {
    return Numeric(m_value);
}

bool AstNodeDouble::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    return m_value < dynamic_cast<const AstNodeDouble*>(rhs)->m_value;
}
