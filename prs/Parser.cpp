//
// Created by pc on 07-06-21.
//

#include "Parser.h"

#include "ParserInfo.h"
#include "StructuralTokenizer.h"
#include "Tokenizer.h"

std::unique_ptr<AstToken> Parser::parse(const std::string& string, ParserInfo& info) {
    try {
        Tokenizer tokenizer(string, info);
        if (info.success()) {
            StructuralTokenizer structuralTokenizer(tokenizer.tokenList(), info);
            if (info.success()) {
                return std::make_unique<AstToken>(structuralTokenizer.tokenList(), info);
            }
        }
        return nullptr;
    } catch (...) { return nullptr; }
}
