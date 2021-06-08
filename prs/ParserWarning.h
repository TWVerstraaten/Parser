//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSERWARNING_H
#define PARSER_PARSERWARNING_H

#include "Range.h"

#include <string>

class ParserWarning {

  public:
    enum WARNING_TYPE { SUSPICIOUS_IDENTIFIER_NUM_PATTERN };

    [[nodiscard]] std::string toString() const;

    ParserWarning(WARNING_TYPE type, std::string message, Range range);

  private:
    WARNING_TYPE m_type;
    std::string  m_message;
    Range        m_range;
};

#endif // PARSER_PARSERWARNING_H
