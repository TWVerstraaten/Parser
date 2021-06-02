//
// Created by pc on 02-06-21.
//

#ifndef PARSER_RESERVEDFUNCTION2_H
#define PARSER_RESERVEDFUNCTION2_H

#include "ReservedFunction.h"

#include <functional>

namespace ast {
    class ReservedFunction2 : public ReservedFunction {
      public:
        template <class Func>
        explicit ReservedFunction2(Func function)
            : m_function{[=](const gen::Number& var, const gen::Number& var2) -> gen::Number {
                  return std::visit([&](const auto& a, const auto& b) { return gen::Number(function(a, b)); }, var.number(), var2.number());
              }} {
        }

        [[nodiscard]] gen::Number eval(const gen::Number& number, const gen::Number& number2) const override;

      private:
        const std::function<gen::Number(const gen::Number&, const gen::Number&)> m_function;
    };

} // namespace ast

#endif // PARSER_RESERVEDFUNCTION2_H
