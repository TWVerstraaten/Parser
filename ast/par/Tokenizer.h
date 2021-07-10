//
// Created by pc on 06-06-21.
//

#ifndef PRS_TOKENIZER_H
#define PRS_TOKENIZER_H

#include "Token.h"

#include <list>
#include <string>

namespace ast::err {
    class ParserInfo;
}

namespace ast::par {
    class Tokenizer {
      public:
        Tokenizer(std::string string, ast::err::ParserInfo& info);

        [[nodiscard]] std::string             toString() const;
        [[nodiscard]] const std::list<Token>& tokenList() const;

      private:
        void tokenize();
        void replaceUnaryMinuses();
        void findIllegalCharacters();
        void checkBrackets();
        void checkDoubleEquals();
        void checkRepeatedCommas();
        void checkRepeatedOperators();
        void checkIdentifierNumberPatternWithNoSpace();

        const std::string     m_string;
        std::list<Token>      m_tokenList;
        ast::err::ParserInfo& m_info;
    };
} // namespace ast::par

#endif // PRS_TOKENIZER_H
