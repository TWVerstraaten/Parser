//
// Created by pc on 08-06-21.
//

#include "CustomFunctionToken.h"

namespace ast::par {

    CustomFunctionToken::CustomFunctionToken(std::string name, size_t argumentCount) : m_name(std::move(name)), m_argumentCount(argumentCount) {
    }

    const std::string& CustomFunctionToken::name() const {
        return m_name;
    }

    size_t CustomFunctionToken::argumentCount() const {
        return m_argumentCount;
    }

    void CustomFunctionToken::setArgumentCount(size_t argumentCount) {
        m_argumentCount = argumentCount;
    }

    std::string CustomFunctionToken::toString() const {
        return m_name + "(" + std::to_string(m_argumentCount) + ")";
    }

    bool operator<(const CustomFunctionToken& left, const CustomFunctionToken& right) {
        if (left.m_name != right.m_name) {
            return left.m_name < right.m_name;
        } else {
            return left.m_argumentCount < right.m_argumentCount;
        }
    }

    bool operator==(const CustomFunctionToken& r, const CustomFunctionToken& l) {
        return l.m_name == r.m_name && l.m_argumentCount == r.m_argumentCount;
    }
} // namespace ast::par