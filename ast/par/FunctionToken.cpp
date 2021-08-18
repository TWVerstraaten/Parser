//
// Created by pc on 08-06-21.
//

#include "FunctionToken.h"

namespace ast::par {

    FunctionToken::FunctionToken(std::string name, size_t argumentCount) : m_name(std::move(name)), m_argumentCount(argumentCount) {
    }

    const std::string& FunctionToken::name() const {
        return m_name;
    }

    size_t FunctionToken::argumentCount() const {
        return m_argumentCount;
    }

    void FunctionToken::setArgumentCount(size_t argumentCount) {
        m_argumentCount = argumentCount;
    }

    std::string FunctionToken::toString() const {
        return m_name + "(" + std::to_string(m_argumentCount) + ")";
    }

    bool operator<(const FunctionToken& left, const FunctionToken& right) {
        if (left.m_name != right.m_name) {
            return left.m_name < right.m_name;
        } else {
            return left.m_argumentCount < right.m_argumentCount;
        }
    }

    bool operator==(const FunctionToken& r, const FunctionToken& l) {
        return l.m_name == r.m_name && l.m_argumentCount == r.m_argumentCount;
    }
} // namespace ast::par