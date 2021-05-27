//
// Created by pc on 27-05-21.
//

#ifndef PARSER_FORMULA_H
#define PARSER_FORMULA_H

#include "Ast.h"
#include "FormulaHeader.h"

#include <memory>
#include <string>

class Formula {

  public:
    explicit Formula(std::string string);

    [[nodiscard]] bool               parseSuccessful() const;
    [[nodiscard]] const std::string& errorString() const;

  private:
    bool        m_parseSuccessful = true;
    std::string m_errorString;

    std::string                    m_string;
    std::unique_ptr<Ast>           m_ast;
    std::unique_ptr<FormulaHeader> m_formulaHeader;
};

#endif // PARSER_FORMULA_H
