//
// Created by pc on 02-06-21.
//

#include "ReservedFunction2.h"

gen::Number ast::ReservedFunction2::eval(const gen::Number& number, const gen::Number& number2) const {
    return m_function(number, number2);
}
