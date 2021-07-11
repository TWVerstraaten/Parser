//
// Created by pc on 08-06-21.
//

#ifndef AST_AST_H
#define AST_AST_H

#include "Header.h"
#include "err/ParserInfo.h"
#include "par/AstToken.h"

namespace ast {
    class Ast {

        Ast() = default;

      public:
        explicit Ast(const std::string& string);

        void replaceVariableInPlace(const std::string& variable, const par::AstToken& token);
        void replaceFunctionInPlace(const Ast& functionToken);

        [[nodiscard]] par::CustomFunctionToken           getCustomFunctionToken() const;
        [[nodiscard]] bool                               success() const;
        [[nodiscard]] bool                               hasCustomDependencies() const;
        [[nodiscard]] const err::ParserInfo&             info() const;
        [[nodiscard]] const Header&                      header() const;
        [[nodiscard]] std::set<par::CustomFunctionToken> functionDependencies() const;
        [[nodiscard]] std::set<std::string>              constantDependencies() const;
        [[nodiscard]] std::set<std::string>              variablesUsed() const;
        [[nodiscard]] std::vector<std::string>           declaredVariables() const;
        [[nodiscard]] const par::AstToken&               body() const;
        [[nodiscard]] Ast                                replaceVariable(const std::string& variable, const ast::par::AstToken& token) const;
        [[nodiscard]] Ast                                replaceFunction(const Ast& functionToken) const;
        [[nodiscard]] std::string                        toStringAsTree() const;
        [[nodiscard]] std::string                        toStringFlat() const;

      private:
        void checkAndSetHeader();
        void buildNonEmptyHeader();
        void checkArgumentsIfFullHeader();
        void checkIfArgumentsAreStrings();
        void checkRepeatedArguments();
        void checkUnusedArguments();

        bool            m_headerWasSet = false;
        err::ParserInfo m_info;
        Header          m_header;
        par::AstToken   m_rootNode;
    };
} // namespace ast

#endif // AST_AST_H
