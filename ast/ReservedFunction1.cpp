//
// Created by pc on 02-06-21.
//

#include "ReservedFunction1.h"

#include <cassert>

gen::Number ast::ReservedFunction1::eval(const gen::Number& number) const {
    return m_function(number);
}
