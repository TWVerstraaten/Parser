//
// Created by pc on 07-06-21.
//

#ifndef PRS_PARSER_H
#define PRS_PARSER_H

#include "AstToken.h"

#include <string>

class ParserInfo;

class Parser {

  public:
    static std::optional<AstToken> parse(const std::string& string, ParserInfo& info);
};

#endif // PRS_PARSER_H
