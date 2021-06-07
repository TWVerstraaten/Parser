//
// Created by pc on 06-06-21.
//

#include "Token.h"

Token::Token(Token::TOKEN_TYPE type, std::string value, size_t startIndexInString, size_t endIndexInString, size_t additional)
    : m_type(type), m_string(std::move(value)), m_startIndexInString(startIndexInString), m_endIndexInString(endIndexInString),
      m_additional(additional) {
}

std::string Token::toString(bool readable) const {
    if (readable) {
        return m_string;
    } else {
        switch (m_type) {
            case TOKEN_TYPE::NUM:
                return "_Num[" + m_string + "] ";
            case TOKEN_TYPE::IDENTIFIER:
                return "_Id[" + m_string + "](" + std::to_string(m_startIndexInString) + ", " + std::to_string(m_endIndexInString) + ") ";
            case TOKEN_TYPE::UNARY_MINUS:
                return "_Un[-] ";
            default:
                break;
        }
        return "[" + m_string + "] ";
    }
}

void Token::setAdditional(size_t additional) {
    m_additional = additional;
}
