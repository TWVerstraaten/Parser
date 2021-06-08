//
// Created by pc on 08-06-21.
//

#ifndef PRS_CUSTOMFUNCTION_H
#define PRS_CUSTOMFUNCTION_H

#include <string>

struct CustomFunction {
    friend bool operator<(const CustomFunction& left, const CustomFunction& right);

    std::string m_name;
    size_t      m_argumentCount;
};

#endif // PRS_CUSTOMFUNCTION_H
