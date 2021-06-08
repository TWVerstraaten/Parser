//
// Created by pc on 06-06-21.
//

#include "Token.h"

Token::Token(Token::TOKEN_TYPE type, std::string value, Range range) : m_type(type), m_string(std::move(value)), m_range(range) {
}

std::string Token::toString(bool readable) const {
    if (readable) {
        return m_string;
    } else {
        switch (m_type) {
            case TOKEN_TYPE::NUMBER:
                return "_Num[" + m_string + "] ";
            case TOKEN_TYPE::IDENTIFIER:
                return "_Id" + m_range.toString() + "(" + m_string + ") ";
            case TOKEN_TYPE::UNARY_MINUS:
                return "_Un[-] ";
            default:
                break;
        }
        return "[" + m_string + "] ";
    }
}
