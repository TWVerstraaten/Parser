//
// Created by pc on 27-05-21.
//

#ifndef PARSER_FORMULA_H
#define PARSER_FORMULA_H

#include "../ast/Ast.h"
#include "../gen/ErrorBase.h"
#include "../gen/Number.h"
#include "FormulaHeader.h"

#include <map>
#include <memory>
#include <string>

namespace fml {
    class Formula : public gen::ErrorBase {

      public:
        explicit Formula(std::string string);

        [[nodiscard]] const ast::Ast&                 ast() const;
        [[nodiscard]] const FormulaHeader&            formulaHeader() const;
        [[nodiscard]] const std::vector<std::string>& declaredVariables() const;
        [[nodiscard]] std::set<std::string>           referencedVariables() const;
        [[nodiscard]] std::set<std::string>           unusedVariables() const;
        [[nodiscard]] std::set<std::string>           undeclaredVariables() const;
        [[nodiscard]] std::string                     getHints() const;
        [[nodiscard]] gen::Number                     eval(const std::map<std::string, gen::Number>& arguments) const;
        [[nodiscard]] const std::string&              toString() const;
        [[nodiscard]] std::string                     toProcessedString() const;
        [[nodiscard]] FunctionSignature               getSignature() const;

      private:
        void initialCheck();
        bool containsIllegalCharacters();
        bool checkForUndeclaredArguments();
        bool parseAst(const std::string& string);
        bool parseFormulaHeader(const std::string& string);

        std::string                    m_string;
        std::unique_ptr<ast::Ast>      m_ast;
        std::unique_ptr<FormulaHeader> m_formulaHeader;
    };
} // namespace fml

#endif // PARSER_FORMULA_H
