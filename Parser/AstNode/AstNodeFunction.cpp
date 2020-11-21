//
// Created by pc on 20-11-20.
//

#include "AstNodeFunction.h"

#include <utility>

AstNodeFunction::AstNodeFunction(std::string functionName, std::unique_ptr<AstNode>&& argument)
    : m_functionName(std::move(functionName)), m_argument(std::move(argument)) {
}

std::string AstNodeFunction::toString() const {
    return m_functionName + "(" + m_argument->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeFunction::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeFunction(m_functionName, m_argument->copy()));
}
std::unique_ptr<AstNode> AstNodeFunction::simplify() const {
    AstNode* simplifiedNode = new AstNodeFunction(m_functionName, m_argument->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeFunction::type() const {
    return NODE_TYPE::FUNCTION;
}

bool AstNodeFunction::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::FUNCTION) {
        return *m_argument == *dynamic_cast<const AstNodeFunction&>(other).m_argument;
    }
    return false;
}
