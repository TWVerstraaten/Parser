//
// Created by pc on 06-06-21.
//

#ifndef PRS_TOKEN_H
#define PRS_TOKEN_H

#include "Range.h"

#include <string>

class Token {

    friend class Tokenizer;
    friend class StructuralTokenizer;
    friend class StructuralToken;
    friend class AstToken;

  public:
    enum class TYPE { PLUS, MINUS, UNARY_MINUS, POWER, DIVIDE, TIMES, LEFT_BR, RIGHT_BR, COMMA, NUMBER, IDENTIFIER, EQUALS };

    Token(TYPE type, std::string value, Range range);

    [[nodiscard]] std::string toString(bool readable = true) const;
    TYPE                      type() const;

  private:
    TYPE        m_type;
    std::string m_string;
    Range       m_range;
};

#endif // PRS_TOKEN_H
