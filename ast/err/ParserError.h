//
// Created by pc on 06-06-21.
//

#ifndef AST_ERR_PARSERERROR_H
#define AST_ERR_PARSERERROR_H

#include "../par/Range.h"

#include <string>

namespace ast::err {
    class ParserError {

      public:
        enum class TYPE {
            ILLEGAL_CHARACTER,
            UNMATCHED_CLOSING_BR,
            UNMATCHED_OPEN_BR,
            TOO_MANY_EQUALS,
            ILLEGAL_SEQUENCE,
            IDENTIFIER_ERROR,
            WRONG_ARGUMENT_COUNT_RESERVED,
            NUMBER_ERROR,
            UNFINISHED,
            GENERIC
        };

        ParserError(TYPE type, std::string message, par::Range range = {});

        [[nodiscard]] std::string       toString() const;
        [[nodiscard]] const par::Range& range() const;

      private:
        TYPE        m_type;
        std::string m_message;
        par::Range  m_range;
    };
} // namespace ast::err

#endif // AST_ERR_PARSERERROR_H
