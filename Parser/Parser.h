//
// Created by pc on 19-11-20.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "AstNode/AstNode.h"
#include "Tokenizer.h"

#include <string>
#include <tuple>
#include <vector>

class Parser {
  public:
    Parser() = default;

    static AstNode* parse(std::string string);

  private:
    enum class PRECEDENCE_TYPE { EXPRESSION, FACTOR, TERM };

    [[nodiscard]] std::string toString(const Tokenizer::TokenList& tokenList) const;

    static AstNode* parse(const Tokenizer::TokenList& tokenList);
    AstNode*        parseFunctions(Tokenizer::TokenList tokenList);
    AstNode*        parseNoFunctions(Tokenizer::TokenList tokenList);
    AstNode*        parseValueType(const Tokenizer::Token& token);
    AstNode*        parseTerm(Tokenizer::TokenList& tokenList);
    AstNode*        parseLeadingValueType(Tokenizer::TokenList& tokenList);
    AstNode*        parseFactor(Tokenizer::TokenList& tokenList);
    AstNode*        parseExpression(Tokenizer::TokenList& tokenList);
    AstNode*        parseBrackets(Tokenizer::TokenList& tokenList);

    Tokenizer::Token      freshReferenceToken() const;
    std::vector<AstNode*> m_subTokenLists;
};

#endif // PARSER_PARSER_H
