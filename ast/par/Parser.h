//
// Created by pc on 07-06-21.
//

#ifndef AST_PAR_PARSER_H
#define AST_PAR_PARSER_H

#include <optional>
#include <string>

namespace ast::err {
    class ParserInfo;
}

namespace ast::par {
    class AstToken;
    class Parser {

      public:
        static std::optional<AstToken> S_PARSE(const std::string& string, ast::err::ParserInfo& info);
    };
} // namespace ast::par

#endif // AST_PAR_PARSER_H
