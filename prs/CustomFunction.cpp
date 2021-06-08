//
// Created by pc on 08-06-21.
//

#include "CustomFunction.h"

bool operator<(const CustomFunction& left, const CustomFunction& right) {
    if (left.m_name != right.m_name) {
        return left.m_name < right.m_name;
    } else {
        return left.m_argumentCount < right.m_argumentCount;
    }
}
