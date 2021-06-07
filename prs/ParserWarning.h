//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSERWARNING_H
#define PARSER_PARSERWARNING_H

#include <string>
class ParserWarning {

  public:
    enum WARNING_TYPE { SUSPICIOUS_IDENTIFIER_NUM_PATTERN };

    [[nodiscard]] std::string toString() const;

    ParserWarning(WARNING_TYPE type, std::string message, size_t startIndexInString, size_t endIndexInString);

  private:
    WARNING_TYPE m_type;
    std::string  m_message;
    size_t       m_startIndexInString = std::numeric_limits<size_t>::max();
    size_t       m_endIndexInString   = std::numeric_limits<size_t>::max();
};

#endif // PARSER_PARSERWARNING_H
