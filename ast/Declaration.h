//
// Created by pc on 18-08-21.
//

#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "ConstantToken.h"
#include "FunctionToken.h"

#include <variant>

namespace ast {

    class Declaration : public std::variant<ConstantToken, FunctionToken> {

      public:
        explicit Declaration(std::variant<ConstantToken, FunctionToken>&& variant);
        explicit Declaration(ConstantToken&& constantToken);
        explicit Declaration(FunctionToken&& functionToken);

        [[nodiscard]] std::string                                       toString() const;
        [[nodiscard]] std::variant<ConstantToken, FunctionToken>&       get();
        [[nodiscard]] const std::variant<ConstantToken, FunctionToken>& get() const;
    };

} // namespace ast

#endif // AST_DECLARATION_H
