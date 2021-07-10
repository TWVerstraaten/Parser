//
// Created by pc on 14-06-21.
//

#include "UnrolledAst.h"

#include "Ast.h"

#include <cassert>

namespace ast {

    UnrolledAst::UnrolledAst(const Ast& ast) : m_header(ast.header()), m_token(ast.body()) {
    }

    std::string UnrolledAst::toString() const {
        return "Header:\t" + m_header.toString() + "\nBody:\t" + m_token.toString() + "\n";
    }

    par::UnrolledAstToken UnrolledAst::setVariable(const std::string& variable, const gen::Number& number) const {
        auto result = m_token;
        result.setVariableInPlace(variable, number);
        return result;
    }

    par::UnrolledAstToken UnrolledAst::setVariables(const std::map<std::string, gen::Number>& variableMap) const {
        assert(not variableMap.empty());
        auto result = m_token;
        for (const auto& [v, n] : variableMap) {
            result.setVariableInPlace(v, n);
        }
        return result;
    }

    void UnrolledAst::setVariableInPlace(const std::string& variable, const gen::Number& number) {
        m_token.setVariableInPlace(variable, number);
    }

    void UnrolledAst::setVariablesInPlace(const std::map<std::string, gen::Number>& variableMap) {
        assert(not variableMap.empty());
        auto result = m_token;
        for (const auto& [v, n] : variableMap) {
            result.setVariableInPlace(v, n);
        }
    }
} // namespace ast