//
// Created by pc on 19-11-20.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "AstNode/AstNode.h"
#include "Tokenizer.h"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

class Parser {
  public:
    Parser() = default;

    static u_ptr_AstNode parse(const std::string& string);

  private:
    static u_ptr_AstNode parse(const Tokenizer::TokenList& tokenList);
    u_ptr_AstNode        parseFunctions(Tokenizer::TokenList tokenList);
    u_ptr_AstNode        parseValueType(const Tokenizer::Token& token);
    u_ptr_AstNode        parseTerm(Tokenizer::TokenList& tokenList);
    u_ptr_AstNode        parseLeadingValueType(Tokenizer::TokenList& tokenList);
    u_ptr_AstNode        parseFactor(Tokenizer::TokenList& tokenList);
    u_ptr_AstNode        parseExpression(Tokenizer::TokenList& tokenList);
    u_ptr_AstNode        parseBrackets(Tokenizer::TokenList& tokenList);

    [[nodiscard]] Tokenizer::Token freshReferenceToken(size_t offset = 0ul) const;
    [[nodiscard]] std::string      toString(const Tokenizer::TokenList& tokenList) const;

    std::vector<u_ptr_AstNode> m_subExpresssionList;
};

#endif // PARSER_PARSER_H
