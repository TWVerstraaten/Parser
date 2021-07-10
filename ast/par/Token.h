//
// Created by pc on 06-06-21.
//

#ifndef PRS_TOKEN_H
#define PRS_TOKEN_H

#include "../TokenTemplates.h"
#include "Range.h"

#include <string>
#include <variant>

namespace ast {

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
} // namespace ast

#endif // PRS_TOKEN_H
