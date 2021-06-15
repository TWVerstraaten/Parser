//
// Created by pc on 13-06-21.
//

#include "Header.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"
#include "AstToken.h"

#include <cassert>

static const std::set<std::string> s_reservedCoordinates{"x", "y", "z"};

Header::Header(const std::string& name) {
    if (s_reservedCoordinates.find(name) != s_reservedCoordinates.end()) {
        m_header = SingleCoordinateHeader{name};
    } else {
        m_header = OnlyNamedHeader{name};
    }
}

Header::Header(const CustomFunctionToken& customFunction, const AstToken& headerToken) {
    std::vector<std::string> variableNames;
    variableNames.reserve(customFunction.argumentCount());
    for (const auto& child : headerToken.children()) {
        assert(std::holds_alternative<std::string>(child.token()));
        variableNames.emplace_back(std::get<std::string>(child.token()));
    }
    m_header = FullHeader{customFunction.name(), std::move(variableNames)};
}

Header::Header(const VectorToken& vectorToken, const AstToken& headerToken) {
    std::vector<std::string> variableNames;
    variableNames.reserve(vectorToken.m_dimension);
    for (const auto& child : headerToken.children()) {
        assert(std::holds_alternative<std::string>(child.token()));
        assert(s_reservedCoordinates.find(std::get<std::string>(child.token())) != s_reservedCoordinates.end());
        variableNames.emplace_back(std::get<std::string>(child.token()));
    }
    m_header = CoordinateVectorHeader{std::move(variableNames)};
}

Header::HEADER_TYPE Header::type() const {
    return std::visit(Overloaded{[](const EmptyHeader) { return HEADER_TYPE::EMPTY; },
                                 [](const OnlyNamedHeader&) { return HEADER_TYPE::ONLY_NAMED; },
                                 [](const SingleCoordinateHeader&) { return HEADER_TYPE::SINGLE_COORDINATE; },
                                 [](const CoordinateVectorHeader&) { return HEADER_TYPE::COORDINATE_VECTOR; },
                                 [](const FullHeader&) { return HEADER_TYPE::NAMED_AND_VARIABLES_DECLARED; }},
                      m_header);
}

std::string Header::toString() const {
    return std::visit(Overloaded{[](EmptyHeader) { return std::string("Empty header"); },
                                 [](const OnlyNamedHeader& namedHeader) { return "Named:\t" + namedHeader.m_name; },
                                 [](const SingleCoordinateHeader& single) { return "Single coord:\t" + single.m_coordinate; },
                                 [](const CoordinateVectorHeader& vector) { return "Coord. vector:\t(" + alg::StringAlg::concatenateStrings(vector.m_coordinates) + ")"; },
                                 [](const FullHeader& h) { return std::string("Full:\t") + h.m_name + "(" + alg::StringAlg::concatenateStrings(h.m_variables) + ")"; }},
                      m_header);
}
