//
// Created by pc on 07-06-21.
//

#ifndef PARSER_PARSERINFO_H
#define PARSER_PARSERINFO_H

#include "ParserError.h"
#include "ParserWarning.h"

#include <vector>

class ParserInfo {

  public:
    void addError(ParserError&& error);
    void addWarning(ParserWarning&& warning);

    [[nodiscard]] bool                              success() const;
    [[nodiscard]] bool                              hasWarning() const;
    [[nodiscard]] const std::vector<ParserError>&   errors() const;
    [[nodiscard]] const std::vector<ParserWarning>& warnings() const;

  private:
    std::vector<ParserError>   m_errors;
    std::vector<ParserWarning> m_warnings;
};

#endif // PARSER_PARSERINFO_H
