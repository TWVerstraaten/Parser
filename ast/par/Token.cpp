//
// Created by pc on 06-06-21.
//

#include "Token.h"

namespace ast::par {

    Token::Token(Token::TYPE type, std::string value, Range range) : m_type(type), m_string(std::move(value)), m_range(range) {
    }

    std::string Token::toString() const {
        switch (m_type) {
            case TYPE::NUMBER:
                return "_Num[" + m_string + "] ";
            case TYPE::IDENTIFIER:
                return "_Id" + m_range.toString() + "(" + m_string + ") ";
            case TYPE::UNARY_MINUS:
                return "_Un[-] ";
            default:
                break;
        }
        return "[" + m_string + "] ";
    }

    Token::TYPE Token::type() const {
        return m_type;
    }

    const Range& Token::range() const {
        return m_range;
    }

    const std::string& Token::string() const {
        return m_string;
    }

    void Token::setType(Token::TYPE type) {
        m_type = type;
    }
} // namespace ast::par