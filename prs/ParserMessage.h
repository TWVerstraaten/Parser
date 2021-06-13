//
// Created by pc on 08-06-21.
//

#ifndef PRS_PARSERMESSAGE_H
#define PRS_PARSERMESSAGE_H

#include "Range.h"

#include <string>

class ParserMessage {

    friend class Tokenizer;
    friend class StructuralTokenizer;

    enum TYPE { INSERT_MULTIPLICATION };

  public:
    ParserMessage(TYPE type, std::string message, Range range = {});

    [[nodiscard]] std::string toString() const;

  private:
    const TYPE        m_type;
    const std::string m_message;
    const Range       m_range;
};

#endif // PRS_PARSERMESSAGE_H
