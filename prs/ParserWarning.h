//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSERWARNING_H
#define PARSER_PARSERWARNING_H

#include "Range.h"

#include <string>

struct ParserWarning {

    enum TYPE { SUSPICIOUS_IDENTIFIER_NUM_PATTERN };

    [[nodiscard]] std::string toString() const;

    const TYPE        m_type;
    const std::string m_message;
    const Range       m_range;
};

#endif // PARSER_PARSERWARNING_H
