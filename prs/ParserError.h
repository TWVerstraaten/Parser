//
// Created by pc on 06-06-21.
//

#ifndef PRS_PARSERERROR_H
#define PRS_PARSERERROR_H

#include "Range.h"

#include <string>

class ParserError {

    friend class Tokenizer;
    friend class StructuralTokenizer;
    friend class AstToken;

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

  public:
    ParserError(TYPE type, std::string message, Range range = {});

    [[nodiscard]] std::string toString() const;

  private:
    const TYPE        m_type;
    const std::string m_message;
    const Range       m_range;
};

#endif // PRS_PARSERERROR_H
