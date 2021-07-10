//
// Created by pc on 06-06-21.
//

#ifndef PRS_PARSERERROR_H
#define PRS_PARSERERROR_H

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

        ParserError(TYPE type, std::string message, Range range = {});

        [[nodiscard]] std::string toString() const;

      private:
        TYPE        m_type;
        std::string m_message;
        Range       m_range;
    };
} // namespace err

#endif // PRS_PARSERERROR_H
