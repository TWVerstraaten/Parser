//
// Created by pc on 07-06-21.
//

#ifndef PRS_PARSERWARNING_H
#define PRS_PARSERWARNING_H

#include "Range.h"

#include <string>

class ParserWarning {

    friend class Tokenizer;

    enum TYPE { SUSPICIOUS_IDENTIFIER_NUM_PATTERN };

  public:
    ParserWarning(TYPE type, const std::string& message, Range range = {});

    [[nodiscard]] std::string toString() const;

  private:
    const TYPE        m_type;
    const std::string m_message;
    const Range       m_range;
};

#endif // PRS_PARSERWARNING_H
