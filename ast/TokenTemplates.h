//
// Created by pc on 13-06-21.
//

#ifndef PRS_TOKENTEMPLATES_H
#define PRS_TOKENTEMPLATES_H

#include <variant>

namespace ast {
    class TokenTemplates {

      public:
        template <class T, class... Ts, class Type>
        [[nodiscard]] static bool tokenEquals(const std::variant<Ts...>& variant, Type type) {
            return std::holds_alternative<T>(variant) && std::get<T>(variant) == type;
        }

        template <class T, class... Ts, class Type>
        [[nodiscard]] static bool isTokenOfType(const std::variant<Ts...>& variant, Type type) {
            return std::holds_alternative<T>(variant) && std::get<T>(variant).type() == type;
        }
    };
} // namespace ast

#endif // PRS_TOKENTEMPLATES_H
