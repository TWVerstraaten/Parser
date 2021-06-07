//
// Created by pc on 06-06-21.
//

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

#include "ParserError.h"
#include "StructuralToken.h"
#include "Token.h"

#include <list>
#include <string>
#include <vector>

class Tokenizer {

  public:
    explicit Tokenizer(std::string string);

    [[nodiscard]] bool                            success() const;
    [[nodiscard]] std::string                     tokensToString(bool readable = true) const;
    [[nodiscard]] std::string                     structuralTokensToString() const;
    [[nodiscard]] const std::string&              string() const;
    [[nodiscard]] const std::list<Token>&         tokenList() const;
    [[nodiscard]] const std::vector<ParserError>& errors() const;

    static std::string toString(const std::list<StructuralToken>& structuralTokenList);

  private:
    void tokenize();
    void replaceUnaryMinuses();
    void findIllegalCharacters();
    void checkBrackets();
    void checkDoubleEquals();
    void checkRepeatedCommas();
    void checkRepeatedOperators();
    void initializeStructuralTokens();
    void extractFunctionsAndBracketsFromStructuralTokens();

    static StructuralToken listToStructuralToken(std::list<StructuralToken> structuralTokenList);

    bool                       m_success = true;
    const std::string          m_string;
    std::list<Token>           m_tokenList;
    std::list<StructuralToken> m_structuralTokenList;
    std::vector<ParserError>   m_errors;
};

#endif // PARSER_TOKENIZER_H
