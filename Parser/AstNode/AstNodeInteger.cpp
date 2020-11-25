//
// Created by pc on 19-11-20.
//

#include "AstNodeInteger.h"

#include <cassert>

AstNodeInteger::AstNodeInteger(const std::string& string) : m_value(stoul(string)) {
}

AstNodeInteger::AstNodeInteger(long long int val) : m_value(val) {
}

std::string AstNodeInteger::toString() const {
    return std::to_string(m_value);
}

std::unique_ptr<AstNode> AstNodeInteger::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeInteger(m_value));
}

std::unique_ptr<AstNode> AstNodeInteger::simplify() const {
    return copy();
}

AstNode::NODE_TYPE AstNodeInteger::type() const {
    return NODE_TYPE::INTEGER;
}

long long AstNodeInteger::value() const {
    return m_value;
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
