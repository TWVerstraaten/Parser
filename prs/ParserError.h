//
// Created by pc on 06-06-21.
//

#ifndef PARSER_PARSERERROR_H
#define PARSER_PARSERERROR_H

#include "Range.h"

#include <string>

class ParserError {

  public:
    enum class ERROR_TYPE {
        ILLEGAL_CHARACTER,
        UNMATCHED_CLOSING_BR,
        UNMATCHED_OPEN_BR,
        TOO_MANY_EQUALS,
        ILLEGAL_SEQUENCE,
        IDENTIFIER_ERROR,
        NUMBER_ERROR
    };

    ParserError(ERROR_TYPE type, std::string message, Range range);

    [[nodiscard]] std::string toString() const;

  private:
    ERROR_TYPE  m_type;
    std::string m_message;
    Range       m_range;
};

#endif // PARSER_PARSERERROR_H
