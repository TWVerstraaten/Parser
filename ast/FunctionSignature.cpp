//
// Created by pc on 02-06-21.
//

#include "FunctionSignature.h"

ast::FunctionSignature::FunctionSignature(std::string functionName, size_t argumentCount)
    : m_functionName(std::move(functionName)), m_argumentCount(argumentCount) {
}

const std::string& ast::FunctionSignature::functionName() const {
    return m_functionName;
}

size_t ast::FunctionSignature::argumentCount() const {
    return m_argumentCount;
}
bool ast::FunctionSignature::operator<(const ast::FunctionSignature& rhs) const {
    if (m_functionName != rhs.m_functionName) {
        return m_functionName < rhs.m_functionName;
    }
    return m_argumentCount < rhs.m_argumentCount;
}
