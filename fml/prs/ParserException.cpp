//
// Created by pc on 27-05-21.
//

#include "ParserException.h"

namespace fml::prs {
    ParserException::ParserException(const ParserException::PARSER_ERROR error, std::string additionalString)
        : m_error(error), m_additionalString(std::move(additionalString)) {
    }

    static std::string errorTypeToString(ParserException::PARSER_ERROR error) {
        switch (error) {
            case ParserException::PARSER_ERROR::OTHER:
                return "Other";
            case ParserException::PARSER_ERROR::ILLEGAL_SYMBOL:
                return "Illegal symbol";
            case ParserException::PARSER_ERROR::VALUE_TYPE_READ:
                return "Can't read value type";
            case ParserException::PARSER_ERROR::INVALID_TERM_OP:
                return "Expected + or -";
            case ParserException::PARSER_ERROR::INVALID_FACTOR_OP:
                return "Expected * or /";
            case ParserException::PARSER_ERROR::INVALID_EXPR_OP:
                return "Expected ^";
            case ParserException::PARSER_ERROR::UNTERMINATED:
                return "Formula not terminated";
            case ParserException::PARSER_ERROR::BRACKET_MISMATCH:
                return "Brackets do not match";
            case ParserException::PARSER_ERROR::RESERVED_FUNCTION_WRONG_ARG_COUNT:
                return "Wrong argument count";
            case ParserException::PARSER_ERROR::RESERVED_FUNCTION_NOT_IMPLEMENTED:
                return "Reserved function not implemented";
        }
        return "";
    }

    std::string ParserException::toString() const {
        return errorTypeToString(m_error) + " " + " " + m_additionalString;
    }
} // namespace fml::prs