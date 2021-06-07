//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "ParserError.h"
#include "ParserInfo.h"
#include "ParserWarning.h"

#include <string>
#include <vector>

class Parser {

  public:
    explicit Parser(std::string string);

    [[nodiscard]] bool                              success() const;
    [[nodiscard]] bool                              hasWarnings() const;
    [[nodiscard]] const std::vector<ParserError>&   errors() const;
    [[nodiscard]] const std::vector<ParserWarning>& warnings() const;
    [[nodiscard]] const std::string&                originalString() const;

  private:
    std::string m_string;
    ParserInfo  m_info;
};

#endif // PARSER_PARSER_H
