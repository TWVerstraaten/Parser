//
// Created by pc on 07-06-21.
//

#ifndef PRS_PARSERINFO_H
#define PRS_PARSERINFO_H

#include "ParserError.h"
#include "ParserMessage.h"
#include "ParserWarning.h"

#include <vector>

namespace ast::err {
    class ParserInfo {

      public:
        void addError(ParserError&& error);
        void addWarning(ParserWarning&& warning);
        void addMessage(ParserMessage&& warning);

        [[nodiscard]] bool                              success() const;
        [[nodiscard]] bool                              hasErrors() const;
        [[nodiscard]] bool                              hasWarnings() const;
        [[nodiscard]] bool                              hasMessages() const;
        [[nodiscard]] const std::vector<ParserError>&   errors() const;
        [[nodiscard]] const std::vector<ParserWarning>& warnings() const;
        [[nodiscard]] const std::vector<ParserMessage>& messages() const;

        void printAll() const;

      private:
        std::vector<ParserError>   m_errors;
        std::vector<ParserWarning> m_warnings;
        std::vector<ParserMessage> m_messages;
    };
} // namespace err

#endif // PRS_PARSERINFO_H
