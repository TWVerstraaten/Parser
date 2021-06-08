//
// Created by pc on 06-06-21.
//

#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

#include "Range.h"

#include <string>

class Token {

    friend class Tokenizer;
    friend class StructuralTokenizer;
    friend class StructuralToken;
    friend class AstToken;

  public:
    enum class TOKEN_TYPE { PLUS, MINUS, UNARY_MINUS, POWER, DIVIDE, TIMES, LEFT_BR, RIGHT_BR, COMMA, NUMBER, IDENTIFIER, EQUALS };

    Token(TOKEN_TYPE type, std::string value, Range range);

    [[nodiscard]] std::string toString(bool readable = true) const;

  private:
    TOKEN_TYPE  m_type;
    std::string m_string;
    Range       m_range;
};

#endif // PARSER_TOKEN_H
