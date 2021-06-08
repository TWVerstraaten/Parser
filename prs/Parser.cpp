//
// Created by pc on 07-06-21.
//

#include "Parser.h"

#include "StructuralTokenizer.h"
#include "Tokenizer.h"

std::unique_ptr<AstToken> Parser::parse(const std::string& string, ParserInfo& info) {
    try {
        Tokenizer           tok(string, info);
        StructuralTokenizer structuralTokenizer(tok.tokenList(), info);
        return std::make_unique<AstToken>(structuralTokenizer.tokenList());
    } catch (...) { return nullptr; }
}
