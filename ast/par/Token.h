//
// Created by pc on 06-06-21.
//

#ifndef AST_PAR_TOKEN_H
#define AST_PAR_TOKEN_H

#include "Range.h"

#include <string>
#include <variant>

namespace ast::par {

    class Token {

      public:
        enum class TYPE { PLUS, MINUS, UNARY_MINUS, POWER, DIVIDE, TIMES, LEFT_BR, RIGHT_BR, COMMA, NUMBER, IDENTIFIER, EQUALS };

        Token(TYPE type, std::string value, Range range);

        void                             setType(TYPE type);
        [[nodiscard]] std::string        toString() const;
        [[nodiscard]] TYPE               type() const;
        [[nodiscard]] const Range&       range() const;
        [[nodiscard]] const std::string& string() const;

      private:
        TYPE        m_type;
        std::string m_string;
        Range       m_range;
    };
} // namespace ast::par

#endif // AST_PAR_TOKEN_H
