//
// Created by pc on 18-08-21.
//

#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "par/ConstantToken.h"
#include "par/FunctionToken.h"

#include <variant>

namespace ast {
    
    class Declaration : public std::variant<par::ConstantToken, par::FunctionToken> {

      public:
        explicit Declaration(std::variant<par::ConstantToken, par::FunctionToken>&& variant);
        explicit Declaration(par::ConstantToken&& constantToken);
        explicit Declaration(par::FunctionToken&& functionToken);

        [[nodiscard]] std::string                                                 toString() const;
        [[nodiscard]] std::variant<par::ConstantToken, par::FunctionToken>&       get();
        [[nodiscard]] const std::variant<par::ConstantToken, par::FunctionToken>& get() const;
    };

} // namespace ast

#endif // AST_DECLARATION_H
