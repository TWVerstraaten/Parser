//
// Created by pc on 27-05-21.
//

#ifndef PARSER_PARSEREXCEPTION_H
#define PARSER_PARSEREXCEPTION_H

#include <stdexcept>
#include <string>

namespace fml::prs {
    class ParserException : public std::exception {

      public:
        enum class PARSER_ERROR {
            OTHER,
            ILLEGAL_SYMBOL,
            VALUE_TYPE_READ,
            INVALID_TERM_OP,
            INVALID_FACTOR_OP,
            RESERVED_FUNCTION_WRONG_ARG_COUNT,
            RESERVED_FUNCTION_NOT_IMPLEMENTED,
            INVALID_EXPR_OP,
            UNTERMINATED,
            BRACKET_MISMATCH
        };

        explicit ParserException(PARSER_ERROR error, std::string additionalString = "");

        [[nodiscard]] std::string toString() const;

      private:
        const PARSER_ERROR m_error;
        const std::string  m_additionalString;
    };
} // namespace fml::prs

#endif // PARSER_PARSEREXCEPTION_H
