//
// Created by pc on 19-11-20.
//

#include "AstNodeVar.h"

#include <utility>

AstNodeVar::AstNodeVar(std::string value) : m_varName(std::move(value)) {
}

std::string AstNodeVar::toString() const {
    return m_varName;
}

std::unique_ptr<AstNode> AstNodeVar::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeVar(m_varName));
}

std::unique_ptr<AstNode> AstNodeVar::simplify() const {
    return copy();
}

AstNode::NODE_TYPE AstNodeVar::type() const {
    return NODE_TYPE::VARIABLE;
}
