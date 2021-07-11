//
// Created by pc on 07-06-21.
//

#ifndef AST_PAR_STRUCTURALTOKENIZER_H
#define AST_PAR_STRUCTURALTOKENIZER_H

#include "StructuralToken.h"
#include "Token.h"

#include <list>
#include <optional>
#include <string>
#include <vector>

namespace ast::err {
    class ParserInfo;
}

namespace ast::par {

    class StructuralTokenizer {

      public:
        StructuralTokenizer(const std::list<Token>& rawTokenList, ast::err::ParserInfo& info);

        void                                            extractFunctionsAndBracketsFromStructuralTokens();
        [[nodiscard]] std::string                       toString() const;
        [[nodiscard]] const std::list<StructuralToken>& tokenList() const;

        [[nodiscard]] static std::string S_TO_STRING(const std::list<StructuralToken>& structuralTokenList);

      private:
        void addStringTokenToStructuralTokens(const Token& token);
        void addNumberTokenToStructuralTokens(const Token& token);
        void insertMultiplicationWhereNeeded(std::list<StructuralToken>& structuralTokens);

        [[nodiscard]] static std::optional<std::string>                     S_PARSE_IDENTIFIER_TOKEN(const Token& token);
        [[nodiscard]] static std::optional<std::variant<double, long long>> S_PARSE_NUMBER_TOKEN(const Token& token);

        ast::err::ParserInfo&      m_info;
        std::list<StructuralToken> m_tokenList;
    };
} // namespace ast::par

#endif // AST_PAR_STRUCTURALTOKENIZER_H
