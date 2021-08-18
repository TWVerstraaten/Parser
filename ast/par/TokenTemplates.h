//
// Created by pc on 13-06-21.
//

#ifndef AST_PAR_TOKENTEMPLATES_H
#define AST_PAR_TOKENTEMPLATES_H

#include <variant>

namespace ast::par {
    class TokenTemplates {

      public:

        template <class T, class... Ts, class Type>
        [[nodiscard]] static bool S_IS_TOKEN_OF_TYPE(const std::variant<Ts...>& variant, Type type) {
            return std::holds_alternative<T>(variant) && std::get<T>(variant).type() == type;
        }
    };
} // namespace ast::par

#endif // AST_PAR_TOKENTEMPLATES_H
