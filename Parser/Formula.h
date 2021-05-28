//
// Created by pc on 27-05-21.
//

#ifndef PARSER_FORMULA_H
#define PARSER_FORMULA_H

#include "Ast.h"
#include "ErrorBase.h"
#include "FormulaHeader.h"

#include <memory>
#include <string>

class Formula : public ErrorBase {

  public:
    explicit Formula(std::string string);

    [[nodiscard]] const Ast&           ast() const;
    [[nodiscard]] const FormulaHeader& formulaHeader() const;

    [[nodiscard]] const std::set<std::string>& declaredVariables() const;
    [[nodiscard]] std::set<std::string>        referencedVariables() const;
    [[nodiscard]] std::set<std::string>        unusedVariables() const;
    [[nodiscard]] std::set<std::string>        undeclaredVariables() const;
    [[nodiscard]] std::string                  getHints() const;

  private:
    bool containsIllegalCharacters();
    bool checkForUndeclaredArguments();
    bool parseAst(const std::string& string);
    bool parseFormulaHeader(const std::string& string);

    std::string                    m_string;
    std::unique_ptr<Ast>           m_ast;
    std::unique_ptr<FormulaHeader> m_formulaHeader;
};

#endif // PARSER_FORMULA_H
