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

class ParserInfo;

class StructuralTokenizer {

  public:
    StructuralTokenizer(const std::list<Token>& rawTokenList, ParserInfo& info);

    void                                            extractFunctionsAndBracketsFromStructuralTokens();
    [[nodiscard]] std::string                       toString() const;
    [[nodiscard]] const std::list<StructuralToken>& tokenList() const;

    [[nodiscard]] static std::string toString(const std::list<StructuralToken>& structuralTokenList);

  private:
    void addStringTokenToStructuralTokens(const Token& token);
    void addNumberTokenToStructuralTokens(const Token& token);
    void insertMultiplicationWhereNeeded(std::list<StructuralToken>& structuralTokens);

    [[nodiscard]] static std::optional<std::string>                     parseIdentifierToken(const Token& token);
    [[nodiscard]] static std::optional<std::variant<double, long long>> parseNumberToken(const Token& token);

    ParserInfo&                m_info;
    std::list<StructuralToken> m_tokenList;
};


#endif // PRS_STRUCTURALTOKENIZER_H