//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "AstToken.h"

#include <memory>
#include <string>

class ParserInfo;

class Parser {

  public:
    static std::unique_ptr<AstToken> parse(const std::string& string, ParserInfo& info);
};

#endif // PARSER_PARSER_H
