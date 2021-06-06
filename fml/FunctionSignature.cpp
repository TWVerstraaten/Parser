//
// Created by pc on 02-06-21.
//

#include "FunctionSignature.h"

namespace fml {
    FunctionSignature::FunctionSignature(std::string functionName, size_t argumentCount)
        : m_functionName(std::move(functionName)), m_argumentCount(argumentCount) {
    }

    const std::string& FunctionSignature::functionName() const {
        return m_functionName;
    }

    size_t FunctionSignature::argumentCount() const {
        return m_argumentCount;
    }
    bool FunctionSignature::operator<(const FunctionSignature& rhs) const {
        if (m_functionName != rhs.m_functionName) {
            return m_functionName < rhs.m_functionName;
        }
        return m_argumentCount < rhs.m_argumentCount;
    }
} // namespace fml