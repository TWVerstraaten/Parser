//
// Created by pc on 08-06-21.
//

#ifndef AST_ERR_PARSERMESSAGE_H
#define AST_ERR_PARSERMESSAGE_H

#include "../par/Range.h"

#include <string>

namespace ast::err {
    class ParserMessage {

      public:
        enum TYPE { INSERT_MULTIPLICATION, UNUSED_ARGUMENT };

        ParserMessage(TYPE type, std::string message, par::Range range = {});

        [[nodiscard]] std::string       toString() const;
        [[nodiscard]] const par::Range& range() const;

      private:
        TYPE        m_type;
        std::string m_message;
        par::Range  m_range;
    };
} // namespace ast::err

#endif // AST_ERR_PARSERMESSAGE_H
