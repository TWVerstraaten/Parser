//
// Created by pc on 19-11-20.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "../../ast/AstNode.h"
#include "Tokenizer.h"

#include <string>
#include <vector>

namespace fml::prs {
    class Parser {
      public:
        static ast::u_ptr_AstNode parse(const std::string& string);

      private:
        Parser() = default;

        static ast::u_ptr_AstNode parse(const Tokenizer::TokenList& tokenList);
        ast::u_ptr_AstNode        parseFunctions(Tokenizer::TokenList tokenList);
        ast::u_ptr_AstNode        parseValueType(const Tokenizer::Token& token);
        ast::u_ptr_AstNode        parseTerm(Tokenizer::TokenList& tokenList);
        ast::u_ptr_AstNode        parseFactor(Tokenizer::TokenList& tokenList);
        ast::u_ptr_AstNode        parseExpression(Tokenizer::TokenList& tokenList);
        ast::u_ptr_AstNode        parseBrackets(Tokenizer::TokenList& tokenList);

        [[nodiscard]] Tokenizer::Token freshReferenceToken() const;
        [[nodiscard]] std::string      toString(const Tokenizer::TokenList& tokenList) const;

        std::vector<ast::u_ptr_AstNode> m_subExpressionList;
    };
} // namespace fml::prs

#endif // PARSER_PARSER_H
