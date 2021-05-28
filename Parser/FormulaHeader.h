//
// Created by pc on 09-04-21.
//

#ifndef PARSER_FORMULAHEADER_H
#define PARSER_FORMULAHEADER_H

#include "ErrorBase.h"

#include <set>
#include <string>

class FormulaHeader : public ErrorBase {

  public:
    explicit FormulaHeader(std::string headerString);

    [[nodiscard]] const std::string&           name() const;
    [[nodiscard]] const std::string&           headerString() const;
    [[nodiscard]] const std::set<std::string>& variables() const;

  private:
    bool containsIllegalCharacters();

    std::string           m_name;
    std::set<std::string> m_variables;
    std::string           m_headerString;
};

#endif // PARSER_FORMULAHEADER_H
