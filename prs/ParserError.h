//
// Created by pc on 06-06-21.
//

#ifndef PARSER_PARSERERROR_H
#define PARSER_PARSERERROR_H

#include <string>
class ParserError {

  public:
    enum class ERROR_TYPE { ILLEGAL_CHARACTER, UNMATCHED_CLOSING_BR, UNMATCHED_OPEN_BR, TOO_MANY_EQUALS, ILLEGAL_SEQUENCE };

    ParserError(ERROR_TYPE  type,
                std::string message,
                size_t      startIndexInString = std::numeric_limits<size_t>::max(),
                size_t      endIndexInString   = std::numeric_limits<size_t>::max());

    [[nodiscard]] std::string toString() const;

  private:
    ERROR_TYPE  m_type;
    std::string m_message;
    size_t      m_startIndexInString = std::numeric_limits<size_t>::max();
    size_t      m_endIndexInString   = std::numeric_limits<size_t>::max();
};

#endif // PARSER_PARSERERROR_H
