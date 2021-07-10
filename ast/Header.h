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
struct VectorToken;

class Header {

  public:
    struct EmptyHeader {};
    struct ConstantHeader {
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
    enum class HEADER_TYPE { EMPTY, CONSTANT, FULL_HEADER, SINGLE_COORDINATE, COORDINATE_VECTOR };

    typedef std::variant<EmptyHeader, ConstantHeader, FullHeader, SingleCoordinateHeader, CoordinateVectorHeader> HeaderVariant;

    Header() = default;
    explicit Header(const std::string& name);
    Header(const CustomFunctionToken& customFunction, const AstToken& headerToken);
    Header(const VectorToken& vectorToken, const AstToken& headerToken);

    [[nodiscard]] const HeaderVariant& headerVariant() const;
    [[nodiscard]] HEADER_TYPE          type() const;
    [[nodiscard]] std::string          toString() const;

  private:
    HeaderVariant m_header{EmptyHeader{}};
};

#endif // PRS_HEADER_H
