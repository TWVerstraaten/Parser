//
// Created by pc on 08-06-21.
//

#ifndef PARSER_PARSERMESSAGE_H
#define PARSER_PARSERMESSAGE_H

#include "Range.h"

#include <string>

struct ParserMessage {

    enum TYPE { INSERT_MULTIPLICATION };

    [[nodiscard]] std::string toString() const;

    const TYPE        m_type;
    const std::string m_message;
    const Range       m_range;
};

#endif // PARSER_PARSERMESSAGE_H
