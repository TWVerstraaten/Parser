//
// Created by pc on 13-06-21.
//

#ifndef AST_HEADER_H
#define AST_HEADER_H

#include "ConstantToken.h"

#include <string>
#include <variant>
#include <vector>

namespace ast {

    namespace par {
        class AstToken;
        struct VectorToken;
    } // namespace par

    class FunctionToken;
    struct EmptyHeader {};
    typedef ConstantToken ConstantHeader;
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

    typedef std::variant<EmptyHeader, ConstantHeader, FullHeader, SingleCoordinateHeader, CoordinateVectorHeader> HeaderVariant;

    class Header : public HeaderVariant {

      public:
        enum class HEADER_TYPE { EMPTY, CONSTANT, FULL_HEADER, SINGLE_COORDINATE, COORDINATE_VECTOR };

        Header() = default;
        explicit Header(const std::string& name);
        Header(const FunctionToken& customFunction, const ast::par::AstToken& headerToken);
        Header(const par::VectorToken& vectorToken, const ast::par::AstToken& headerToken);

        [[nodiscard]] HEADER_TYPE type() const;
        [[nodiscard]] std::string toString() const;
    };
} // namespace ast

#endif // AST_HEADER_H
