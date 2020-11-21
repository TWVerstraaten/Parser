//
// Created by pc on 19-11-20.
//

#include "AstNodeDouble.h"

AstNodeDouble::AstNodeDouble(const std::string& string) : m_value(stod(string)) {
}

AstNodeDouble::AstNodeDouble(double value) : m_value(value) {
}

std::string AstNodeDouble::toString() const {
    return std::to_string(m_value);
}

std::unique_ptr<AstNode> AstNodeDouble::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeDouble(m_value));
}

std::unique_ptr<AstNode> AstNodeDouble::simplify() const {
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
