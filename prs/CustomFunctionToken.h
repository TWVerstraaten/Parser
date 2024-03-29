//
// Created by pc on 08-06-21.
//

#ifndef PRS_CUSTOMFUNCTIONTOKEN_H
#define PRS_CUSTOMFUNCTIONTOKEN_H

#include <string>

class CustomFunctionToken {
    friend bool operator<(const CustomFunctionToken& left, const CustomFunctionToken& right);

  public:
    CustomFunctionToken(std::string name, size_t argumentCount);

    void                             setArgumentCount(size_t argumentCount);
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] size_t             argumentCount() const;

    friend bool operator==(const CustomFunctionToken& r, const CustomFunctionToken& l);

  private:
    std::string m_name;
    size_t      m_argumentCount;
};

#endif // PRS_CUSTOMFUNCTIONTOKEN_H
