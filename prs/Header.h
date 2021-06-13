//
// Created by pc on 13-06-21.
//

#ifndef PRS_HEADER_H
#define PRS_HEADER_H

#include <string>
#include <variant>
#include <vector>

class Header {

    friend class Ast;

    enum class HEADER_TYPE { EMPTY, ONLY_NAMED, NAMED_AND_VARIABLES_DECLARED };

    struct EmptyHeader {};

    struct OnlyNamedHeader {
        std::string m_name;
    };

    struct NamedAndVariablesHeader {
        std::string              m_name;
        std::vector<std::string> m_variables;
    };

  public:
    Header() = default;
    Header(EmptyHeader empty);
    Header(OnlyNamedHeader&& named);
    Header(NamedAndVariablesHeader&& namedAndVariables);

    [[nodiscard]] HEADER_TYPE type() const;
    [[nodiscard]] std::string toString() const;

  private:
    std::variant<EmptyHeader, OnlyNamedHeader, NamedAndVariablesHeader> m_header{EmptyHeader{}};
};

#endif // PRS_HEADER_H
