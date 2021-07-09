//
// Created by pc on 14-06-21.
//

#include "UnrolledAst.h"

#include "Ast.h"

#include <cassert>

UnrolledAst::UnrolledAst(const Ast& ast) : m_header(ast.header()), m_token(ast.body()) {
}

std::string UnrolledAst::toString() const {
    return "Header:\t" + m_header.toString() + "\nBody:\t" + m_token.toString() + "\n";
}

UnrolledAstToken UnrolledAst::setVariable(const std::string& variable, const gen::Number& number) const {
    return m_token.setVariable(variable, number);
}

UnrolledAstToken UnrolledAst::setVariables(const std::map<std::string, gen::Number>& variableMap) const {
    assert(not variableMap.empty());
    auto result = m_token;
    for (const auto& [v, n] : variableMap) {
        result = result.setVariable(v, n);
    }
    return result;
}
