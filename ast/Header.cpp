//
// Created by pc on 13-06-21.
//

#include "Header.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"
#include "par/AstToken.h"

#include <cassert>

namespace ast {

    static const std::set<std::string> S_RESERVED_COORDINATES{"x", "y", "z"};

    Header::Header(const std::string& name) {
        if (S_RESERVED_COORDINATES.find(name) != S_RESERVED_COORDINATES.end()) {
            m_header = SingleCoordinateHeader{name};
        } else {
            m_header = ConstantHeader{name};
        }
    }

    Header::Header(const par::CustomFunctionToken& customFunction, const ast::par::AstToken& headerToken) {
        std::vector<std::string> variableNames;
        variableNames.reserve(customFunction.argumentCount());
        for (const auto& child : headerToken.children()) {
            assert(std::holds_alternative<std::string>(child.token()));
            variableNames.emplace_back(std::get<std::string>(child.token()));
        }
        m_header = FullHeader{customFunction.name(), std::move(variableNames)};
    }

    Header::Header(const par::VectorToken& vectorToken, const ast::par::AstToken& headerToken) {
        std::vector<std::string> variableNames;
        variableNames.reserve(vectorToken.m_dimension);
        for (const auto& child : headerToken.children()) {
            assert(std::holds_alternative<std::string>(child.token()));
            assert(S_RESERVED_COORDINATES.find(std::get<std::string>(child.token())) != S_RESERVED_COORDINATES.end());
            variableNames.emplace_back(std::get<std::string>(child.token()));
        }
        m_header = CoordinateVectorHeader{std::move(variableNames)};
    }

    Header::HEADER_TYPE Header::type() const {
        return std::visit(Overloaded{[](const EmptyHeader) { return HEADER_TYPE::EMPTY; },
                                     [](const ConstantHeader&) { return HEADER_TYPE::CONSTANT; },
                                     [](const SingleCoordinateHeader&) { return HEADER_TYPE::SINGLE_COORDINATE; },
                                     [](const CoordinateVectorHeader&) { return HEADER_TYPE::COORDINATE_VECTOR; },
                                     [](const FullHeader&) { return HEADER_TYPE::FULL_HEADER; }},
                          m_header);
    }

    std::string Header::toString() const {
        return std::visit(Overloaded{[](EmptyHeader) { return std::string("Empty headerVariant"); },
                                     [](const ConstantHeader& namedHeader) { return "Named:\t" + namedHeader.m_name; },
                                     [](const SingleCoordinateHeader& single) { return "Single coord:\t" + single.m_coordinate; },
                                     [](const CoordinateVectorHeader& vector) { return "Coord. vector:\t(" + alg::str::CONCATENATE_STRINGS(vector.m_coordinates) + ")"; },
                                     [](const FullHeader& h) { return std::string("Full:\t") + h.m_name + "(" + alg::str::CONCATENATE_STRINGS(h.m_variables) + ")"; }},
                          m_header);
    }

    const Header::HeaderVariant& Header::headerVariant() const {
        return m_header;
    }
} // namespace ast