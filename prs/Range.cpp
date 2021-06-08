//
// Created by pc on 07-06-21.
//

#include "Range.h"

std::string Range::toString() const {
    return "[" + std::to_string(m_startIndex) + "," + std::to_string(m_endIndex) + "]";
}
