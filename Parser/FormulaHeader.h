//
// Created by pc on 09-04-21.
//

#ifndef PARSER_FORMULAHEADER_H
#define PARSER_FORMULAHEADER_H

#include <string>
#include <unordered_set>

class FormulaHeader {

  public:
    explicit FormulaHeader(std::string headerString);

    [[nodiscard]] const std::string&                     name() const;
    [[nodiscard]] const std::unordered_set<std::string>& variables() const;
    [[nodiscard]] const std::string&                     headerString() const;

  private:
    std::string                     m_name;
    std::unordered_set<std::string> m_variables;
    std::string                     m_headerString;
};

#endif // PARSER_FORMULAHEADER_H
