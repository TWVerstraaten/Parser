//
// Created by pc on 02-06-21.
//

#ifndef PARSER_RESERVEDFUNCTION1_H
#define PARSER_RESERVEDFUNCTION1_H

#include "../gen/Number.h"
#include "ReservedFunction.h"

#include <functional>

namespace ast {
    class ReservedFunction1 : public ReservedFunction {
      public:
        template <class Func>
        explicit ReservedFunction1(Func function)
            : m_function{[=](const gen::Number& var) -> gen::Number {
                  return std::visit([&](const auto& a) { return gen::Number(function(a)); }, var.number());
              }} {
        }

        ~ReservedFunction1() override = default;

        [[nodiscard]] gen::Number eval(const gen::Number& number) const override;

      private:
        const std::function<gen::Number(const gen::Number&)> m_function;
    };

} // namespace ast

#endif // PARSER_RESERVEDFUNCTION1_H
