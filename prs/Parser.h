//
// Created by pc on 07-06-21.
//

#ifndef PRS_PARSER_H
#define PRS_PARSER_H

#include "AstToken.h"

#include <string>

namespace err {
    class ParserInfo;
}

class Parser {

  public:
    static std::optional<AstToken> S_PARSE(const std::string& string, err::ParserInfo& info);
};

#endif // PRS_PARSER_H
