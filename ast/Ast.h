//
// Created by pc on 08-06-21.
//

#ifndef AST_AST_H
#define AST_AST_H

#include "ConstantToken.h"
#include "Declaration.h"
#include "Dependency.h"
#include "Header.h"
#include "err/ParserInfo.h"
#include "par/AstToken.h"

namespace ast {

    class Ast {

        Ast() = default;

      public:
        explicit Ast(const std::string& string);

        void replaceVariableInPlace(const std::string& variable, const par::AstToken& token);
        void replaceFunctionInPlace(const Ast& function);
        void replaceConstantInPlace(const Ast& constant);
        void replaceDependencyInPlace(const Dependency& dependency, const Ast& ast);

        [[nodiscard]] bool                     isDeclaration() const;
        [[nodiscard]] bool                     isFunction() const;
        [[nodiscard]] bool                     isConstant() const;
        [[nodiscard]] bool                     success() const;
        [[nodiscard]] bool                     isAnonymous() const;
        [[nodiscard]] bool                     isEmpty() const;
        [[nodiscard]] bool                     canUnroll() const;
        [[nodiscard]] bool                     dimensionsMatch() const;
        [[nodiscard]] size_t                   dimension() const;
        [[nodiscard]] Header::HEADER_TYPE      headerType() const;
        [[nodiscard]] FunctionToken            getFunctionToken() const;
        [[nodiscard]] ConstantToken            getConstantToken() const;
        [[nodiscard]] Declaration              getDeclarationToken() const;
        [[nodiscard]] const err::ParserInfo&   info() const;
        [[nodiscard]] const Header&            header() const;
        [[nodiscard]] std::set<FunctionToken>  getFunctionDependencies() const;
        [[nodiscard]] std::set<ConstantToken>  getConstantDependencies() const;
        [[nodiscard]] std::set<Dependency>     getDependencies() const;
        [[nodiscard]] std::set<std::string>    variablesUsed() const;
        [[nodiscard]] std::vector<std::string> declaredVariables() const;
        [[nodiscard]] const par::AstToken&     body() const;
        [[nodiscard]] Ast                      replaceVariable(const std::string& variable, const ast::par::AstToken& token) const;
        [[nodiscard]] Ast                      replaceFunction(const Ast& functionToken) const;
        [[nodiscard]] std::string              toStringAsTree() const;
        [[nodiscard]] std::string              toStringFlat() const;
        [[nodiscard]] std::string              getDeclaredName() const;

      private:
        void buildNonEmptyHeader();
        void checkArgumentsIfFullHeader();
        void checkIfArgumentsAreStrings();
        void checkRepeatedArguments();
        void checkUnusedArguments();

        bool            m_headerWasSet = false;
        err::ParserInfo m_info;
        Header          m_header;
        par::AstToken   m_body;
    };
} // namespace ast

#endif // AST_AST_H
