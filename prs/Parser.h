//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "AstToken.h"
#include "ParserError.h"
#include "ParserInfo.h"
#include "ParserWarning.h"

#include <memory>
#include <string>
#include <vector>

class Parser {

  public:
    explicit Parser(std::string string);

    [[nodiscard]] bool               success() const;
    [[nodiscard]] const std::string& originalString() const;
    [[nodiscard]] const ParserInfo&  info() const;

  private:
    std::unique_ptr<AstToken> m_astToken;
    std::string               m_string;
    ParserInfo                m_info;
};

#endif // PARSER_PARSER_H
