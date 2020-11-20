//
// Created by pc on 19-11-20.
//

#include "AstNodeDouble.h"

AstNodeDouble::AstNodeDouble(const std::string& string) : AstNode(NODE_TYPE::NUM), m_val(stod(string)) {
}

std::string AstNodeDouble::toString() const {
    return std::to_string(m_val);
}
