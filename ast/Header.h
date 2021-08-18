//
// Created by pc on 13-06-21.
//

#ifndef AST_HEADER_H
#define AST_HEADER_H

#include <string>
#include <variant>
#include <vector>

namespace ast {

    namespace par {
        class AstToken;
        struct VectorToken;
        class FunctionToken;
    } // namespace par

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
        Header(const par::FunctionToken& customFunction, const ast::par::AstToken& headerToken);
        Header(const par::VectorToken& vectorToken, const ast::par::AstToken& headerToken);
        ~Header();

        [[nodiscard]] const HeaderVariant& headerVariant() const;
        [[nodiscard]] HEADER_TYPE          type() const;
        [[nodiscard]] std::string          toString() const;

      private:
        HeaderVariant m_header{EmptyHeader{}};
    };
} // namespace ast

#endif // AST_HEADER_H
