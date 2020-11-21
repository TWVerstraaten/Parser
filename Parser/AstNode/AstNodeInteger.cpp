//
// Created by pc on 19-11-20.
//

#include "AstNodeInteger.h"

AstNodeInteger::AstNodeInteger(const std::string& string) : m_value(stoul(string)) {
}

AstNodeInteger::AstNodeInteger(size_t val) : m_value(val) {
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

size_t AstNodeInteger::value() const {
    return m_value;
}
