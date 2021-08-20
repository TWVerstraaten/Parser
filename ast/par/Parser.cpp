//
// Created by pc on 07-06-21.
//

#include "Parser.h"

#include "../err/ParserInfo.h"
#include "AstToken.h"
#include "StructuralTokenizer.h"
#include "Tokenizer.h"


namespace ast::par {

    std::optional<AstToken> Parser::S_PARSE(const std::string& string, ast::err::ParserInfo& info) {
        Tokenizer tokenizer(string, info);
        if (info.success()) {
            StructuralTokenizer structuralTokenizer(tokenizer.tokenList(), info);
            if (info.success()) {
                auto astToken = AstToken{structuralTokenizer.tokenList(), info};
                if (astToken.hasEmptyBody()) {
                    info.add({err::ParserError::TYPE::EMPTY_BODY, "", {0, string.length() - 1}});
                    return {};
                } else {
                    return astToken;
                }
            }
        }
        return {};
    }
} // namespace ast::par