//
// Created by pc on 06-06-21.
//

#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

#include <string>

class Token {

    friend class Tokenizer;
    friend class StructuralToken;

  public:
    enum class TOKEN_TYPE { PLUS, MINUS, UNARY_MINUS, POWER, DIVIDE, TIMES, LEFT_BR, RIGHT_BR, COMMA, NUMBER, IDENTIFIER, EQUALS };

    Token(TOKEN_TYPE  type,
          std::string value,
          size_t      startIndexInString,
          size_t      endIndexInString,
          size_t      additional = std::numeric_limits<size_t>::max());

    void setAdditional(size_t additional);

    [[nodiscard]] std::string toString(bool readable = true) const;

  private:
    TOKEN_TYPE  m_type;
    std::string m_string;
    size_t      m_startIndex;
    size_t      m_endIndex;
    size_t      m_additional;
};

#endif // PARSER_TOKEN_H
