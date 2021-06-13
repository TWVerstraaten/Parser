//
// Created by pc on 08-06-21.
//

#ifndef PRS_CUSTOMFUNCTION_H
#define PRS_CUSTOMFUNCTION_H

#include <string>

class CustomFunction {

    friend class AstToken;
    friend bool operator<(const CustomFunction& left, const CustomFunction& right);

  public:
    CustomFunction(std::string name, size_t argumentCount);

    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] size_t             argumentCount() const;

  private:
    std::string m_name;
    size_t      m_argumentCount;
};

#endif // PRS_CUSTOMFUNCTION_H
