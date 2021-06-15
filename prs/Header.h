//
// Created by pc on 13-06-21.
//

#ifndef PRS_HEADER_H
#define PRS_HEADER_H

#include <string>
#include <variant>
#include <vector>

class CustomFunctionToken;
class AstToken;
class VectorToken;

class Header {

    friend class Ast;

    enum class HEADER_TYPE { EMPTY, ONLY_NAMED, NAMED_AND_VARIABLES_DECLARED, SINGLE_COORDINATE, COORDINATE_VECTOR };

    struct EmptyHeader {};

    struct OnlyNamedHeader {
        std::string m_name;
    };

    struct SingleCoordinateHeader {
        std::string m_coordinate;
    };

    struct CoordinateVectorHeader {
        std::vector<std::string> m_coordinates;
    };

    struct FullHeader {
        std::string              m_name;
        std::vector<std::string> m_variables;
    };

  public:
    Header() = default;
    Header(const std::string& name);
    Header(const CustomFunctionToken& customFunction, const AstToken& headerToken);
    Header(const VectorToken& vectorToken, const AstToken& headerToken);

    [[nodiscard]] HEADER_TYPE type() const;
    [[nodiscard]] std::string toString() const;

  private:
    std::variant<EmptyHeader, OnlyNamedHeader, FullHeader, SingleCoordinateHeader, CoordinateVectorHeader> m_header{EmptyHeader{}};
};

#endif // PRS_HEADER_H
