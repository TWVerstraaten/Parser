//
// Created by pc on 19-11-20.
//

#include "AstNodeVar.h"

AstNodeVar::AstNodeVar(const std::string& value) : AstNode(AstNode::NODE_TYPE::VAR), m_varName(value) {
}

std::string AstNodeVar::toString() const {
    return m_varName;
}
