//
// Created by pc on 14-06-21.
//

#ifndef AST_UNROLLEDAST_H
#define AST_UNROLLEDAST_H

#include "ConstantToken.h"
#include "Header.h"
#include "par/UnrolledAstToken.h"

#include <map>

namespace ast {

    class Ast;

    class UnrolledAst {

      public:
        explicit UnrolledAst(const Ast& ast);

        void setVariableInPlace(const std::string& variable, const gen::Number& number);
        void setVariablesInPlace(const std::map<std::string, gen::Number>& variableMap);

        [[nodiscard]] const par::UnrolledAstToken& token() const;
        [[nodiscard]] par::UnrolledAstToken        setVariable(const std::string& variable, const gen::Number& number) const;
        [[nodiscard]] par::UnrolledAstToken        setVariables(const std::map<std::string, gen::Number>& variableMap) const;
        [[nodiscard]] std::vector<ConstantToken>   declaredVariables() const;

        [[nodiscard]] std::string toString() const;

      private:
        Header                m_header;
        par::UnrolledAstToken m_token;
    };
} // namespace ast

#endif // AST_UNROLLEDAST_H
