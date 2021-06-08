//
// Created by pc on 02-06-21.
//

#ifndef PRS_RESERVEDFUNCTION_H
#define PRS_RESERVEDFUNCTION_H

#include "../gen/Number.h"

namespace ast {

    class ReservedFunction {

      public:
        virtual ~ReservedFunction() = default;

        [[nodiscard]] virtual gen::Number eval(const gen::Number& number) const;
        [[nodiscard]] virtual gen::Number eval(const gen::Number& number1, const gen::Number& number2) const;

      private:
    };

} // namespace ast

#endif // PRS_RESERVEDFUNCTION_H
