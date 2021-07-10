//
// Created by pc on 07-06-21.
//

#ifndef PRS_PARSER_H
#define PRS_PARSER_H

#include "AstToken.h"

#include <string>

namespace ast::err {
    class ParserInfo;
}

namespace ast::par {
    class Parser {

      public:
        static std::optional<AstToken> S_PARSE(const std::string& string, ast::err::ParserInfo& info);
    };
} // namespace ast::par

#endif // PRS_PARSER_H
