//
// Created by pc on 08-06-21.
//

#include "CustomFunction.h"

CustomFunction::CustomFunction(std::string name, size_t argumentCount) : m_name(std::move(name)), m_argumentCount(argumentCount) {
}

bool operator<(const CustomFunction& left, const CustomFunction& right) {
    if (left.m_name != right.m_name) {
        return left.m_name < right.m_name;
    } else {
        return left.m_argumentCount < right.m_argumentCount;
    }
}

const std::string& CustomFunction::name() const {
    return m_name;
}

size_t CustomFunction::argumentCount() const {
    return m_argumentCount;
}
