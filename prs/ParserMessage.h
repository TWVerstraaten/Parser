//
// Created by pc on 08-06-21.
//

#ifndef PRS_PARSERMESSAGE_H
#define PRS_PARSERMESSAGE_H

#include "Range.h"

#include <string>

class ParserMessage {

  public:
    enum TYPE { INSERT_MULTIPLICATION };

    ParserMessage(TYPE type, std::string message, Range range = {});

    [[nodiscard]] std::string toString() const;

  private:
    TYPE        m_type;
    std::string m_message;
    Range       m_range;
};

#endif // PRS_PARSERMESSAGE_H
