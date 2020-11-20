//
// Created by pc on 19-11-20.
//

#include "AstNodeInteger.h"

AstNodeInteger::AstNodeInteger(const std::string& string) : AstNode(NODE_TYPE::NUM), m_val(stoi(string)) {
}

std::string AstNodeInteger::toString() const {
    return std::to_string(m_val);
}
