//
// Created by pc on 06-06-21.
//

#ifndef PARSER_PARSERERROR_H
#define PARSER_PARSERERROR_H

#include "Range.h"

#include <string>

struct ParserError {

    enum class TYPE {
        ILLEGAL_CHARACTER,
        UNMATCHED_CLOSING_BR,
        UNMATCHED_OPEN_BR,
        TOO_MANY_EQUALS,
        ILLEGAL_SEQUENCE,
        IDENTIFIER_ERROR,
        NUMBER_ERROR
    };

    [[nodiscard]] std::string toString() const;

    const TYPE        m_type;
    const std::string m_message;
    const Range       m_range;
};

#endif // PARSER_PARSERERROR_H
