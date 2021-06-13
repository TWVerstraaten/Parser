//
// Created by pc on 13-06-21.
//

#include "Header.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"

Header::Header(Header::EmptyHeader empty) : m_header(empty) {
}

Header::Header(Header::OnlyNamedHeader&& named) : m_header(named) {
}

Header::Header(Header::NamedAndVariablesHeader&& namedAndVariables) : m_header(namedAndVariables) {
}

Header::HEADER_TYPE Header::type() const {
    return std::visit(Overloaded{[](const EmptyHeader) { return HEADER_TYPE::EMPTY; },
                                 [](const OnlyNamedHeader&) { return HEADER_TYPE::ONLY_NAMED; },
                                 [](const NamedAndVariablesHeader&) { return HEADER_TYPE::NAMED_AND_VARIABLES_DECLARED; }},
                      m_header);
}

std::string Header::toString() const {
    return std::visit(Overloaded{[](EmptyHeader) { return std::string("Empty header\n"); },
                                 [](const OnlyNamedHeader& namedHeader) { return "Named:\t" + namedHeader.m_name; },
                                 [](const NamedAndVariablesHeader& h) {
                                     return std::string("Full:\t") + h.m_name + "(" + alg::StringAlg::concatenateStrings(h.m_variables) +
                                            ")";
                                 }},
                      m_header);
}
