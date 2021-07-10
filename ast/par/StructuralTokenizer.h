//
// Created by pc on 07-06-21.
//

#ifndef PRS_STRUCTURALTOKENIZER_H
#define PRS_STRUCTURALTOKENIZER_H

#include "StructuralToken.h"
#include "Token.h"

#include <list>
#include <optional>
#include <string>
#include <vector>

namespace ast {

    namespace err {
        class ParserInfo;
    }

    class StructuralTokenizer {

      public:
        StructuralTokenizer(const std::list<Token>& rawTokenList, err::ParserInfo& info);

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

        err::ParserInfo&           m_info;
        std::list<StructuralToken> m_tokenList;
    };
} // namespace ast

#endif // PRS_STRUCTURALTOKENIZER_H
