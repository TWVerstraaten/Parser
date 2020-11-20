//
// Created by pc on 20-11-20.
//

#include "AstNodeFunction.h"

AstNodeFunction::AstNodeFunction(const std::string& functionName, AstNode* argument)
    : AstNode(AstNode::NODE_TYPE::FUNCTION), m_functionName(functionName), m_argument(argument) {
}

std::string AstNodeFunction::toString() const {
    return m_functionName + "(" + m_argument->toString() + ")";
}
