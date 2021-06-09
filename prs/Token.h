//
// Created by pc on 06-06-21.
//

#ifndef PRS_TOKEN_H
#define PRS_TOKEN_H

#include "Range.h"

#include <string>
#include <variant>

class Token {

    friend class Tokenizer;
    friend class StructuralTokenizer;
    friend class StructuralToken;
    friend class AstToken;
    friend class TokenWriter;

  public:
    enum class TYPE { PLUS, MINUS, UNARY_MINUS, POWER, DIVIDE, TIMES, LEFT_BR, RIGHT_BR, COMMA, NUMBER, IDENTIFIER, EQUALS };

    Token(TYPE type, std::string value, Range range);

    [[nodiscard]] std::string toString() const;
    [[nodiscard]] TYPE        type() const;

    template <class... Ts>
    [[nodiscard]] static bool isTokenOfType(const std::variant<Ts...>& t, Token::TYPE type) {
        return std::holds_alternative<Token>(t) && std::get<Token>(t).m_type == type;
    }

  private:
    TYPE        m_type;
    std::string m_string;
    Range       m_range;
};

#endif // PRS_TOKEN_H
