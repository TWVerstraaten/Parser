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

    static std::unique_ptr<AstNode> parse(const std::string& string);

  private:
    static std::unique_ptr<AstNode> parse(const Tokenizer::TokenList& tokenList);
    std::unique_ptr<AstNode>        parseFunctions(Tokenizer::TokenList tokenList);
    std::unique_ptr<AstNode>        parseValueType(const Tokenizer::Token& token);
    std::unique_ptr<AstNode>        parseTerm(Tokenizer::TokenList& tokenList);
    std::unique_ptr<AstNode>        parseLeadingValueType(Tokenizer::TokenList& tokenList);
    std::unique_ptr<AstNode>        parseFactor(Tokenizer::TokenList& tokenList);
    std::unique_ptr<AstNode>        parseExpression(Tokenizer::TokenList& tokenList);
    std::unique_ptr<AstNode>        parseBrackets(Tokenizer::TokenList& tokenList);

    [[nodiscard]] Tokenizer::Token freshReferenceToken(size_t offset = 0ul) const;
    [[nodiscard]] std::string      toString(const Tokenizer::TokenList& tokenList) const;

    std::vector<std::unique_ptr<AstNode>> m_subTokenLists;
};

#endif // PARSER_PARSER_H
