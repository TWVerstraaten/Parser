//
// Created by pc on 07-06-21.
//

#ifndef AST_ERR_PARSERINFO_H
#define AST_ERR_PARSERINFO_H

#include "ParserError.h"
#include "ParserMessage.h"
#include "ParserWarning.h"

#include <vector>

namespace ast::err {

    class ParserError;
    class ParserMessage;
    class ParserWarning;

    class ParserInfo {

      public:
        ParserInfo();
        ~ParserInfo();

        void add(ParserError&& error);
        void add(ParserWarning&& warning);
        void add(ParserMessage&& warning);

        void printAll() const;
        void clear();

        [[nodiscard]] bool                              success() const;
        [[nodiscard]] bool                              hasErrors() const;
        [[nodiscard]] bool                              hasWarnings() const;
        [[nodiscard]] bool                              hasMessages() const;
        [[nodiscard]] const std::vector<ParserError>&   parserErrors() const;
        [[nodiscard]] const std::vector<ParserWarning>& warnings() const;
        [[nodiscard]] const std::vector<ParserMessage>& messages() const;

      private:
        std::vector<ParserError>   m_parserErrors;
        std::vector<ParserWarning> m_warnings;
        std::vector<ParserMessage> m_messages;
    };
} // namespace ast::err

#endif // AST_ERR_PARSERINFO_H
