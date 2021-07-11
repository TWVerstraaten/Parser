//
// Created by pc on 07-06-21.
//

#include "Parser.h"

#include "../err/ParserInfo.h"
#include "StructuralTokenizer.h"
#include "Tokenizer.h"

namespace ast::par {

    std::optional<AstToken> Parser::S_PARSE(const std::string& string, ast::err::ParserInfo& info) {
        Tokenizer tokenizer(string, info);
        if (info.success()) {
            StructuralTokenizer structuralTokenizer(tokenizer.tokenList(), info);
            if (info.success()) {
                return AstToken{structuralTokenizer.tokenList(), info};
            }
        }
        return {};
    }
} // namespace ast::par