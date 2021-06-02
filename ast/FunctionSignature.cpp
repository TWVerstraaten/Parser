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
