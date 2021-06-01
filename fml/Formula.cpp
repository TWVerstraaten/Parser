//
// Created by pc on 27-05-21.
//

#include "Formula.h"

#include "../alg/BoostWrapper.h"
#include "../alg/StringAlg.h"

#include <algorithm>
#include <cassert>

namespace fml {
    static std::set<char> findIllegalCharacters(const std::string& string) {
        std::set<char>           illegalCharacters;
        static const std::string allowedSpecialCharacters = ".,()+-*/^=\t ";
        for (char c : string) {
            if (not isalnum(c) && allowedSpecialCharacters.find_first_of(c) == std::string::npos) {
                illegalCharacters.insert(c);
            }
        }
        return illegalCharacters;
    }

    Formula::Formula(std::string string) : m_string(std::move(string)) {
        if (setErrorIf(containsIllegalCharacters(), "Contains Illegal characters")) {
            return;
        }
        if (setErrorIf(m_string.empty(), "Empty Formula")) {
            return;
        }
        const auto parts = alg::BoostWrapper::trimAndSplit(m_string, "=");
        if (setErrorIf(parts.size() == 1, "Does not contain '=' sign")) {
            return;
        }
        if (setErrorIf(parts.size() > 2, "Too many '=' signs")) {
            return;
        }
        if (not parseFormulaHeader(parts.at(0)) || not parseAst(parts.at(1))) {
            return;
        }
        checkForUndeclaredArguments();
    }

    const ast::Ast& Formula::ast() const {
        assert(m_ast);
        return *m_ast;
    }

    const FormulaHeader& Formula::formulaHeader() const {
        assert(m_formulaHeader);
        return *m_formulaHeader;
    }

    bool Formula::containsIllegalCharacters() {
        auto illegalCharacters = findIllegalCharacters(m_string);
        if (illegalCharacters.empty()) {
            return false;
        }
        std::string chars;
        std::for_each(illegalCharacters.begin(), illegalCharacters.end(), [&](char c) { chars += c; });
        m_success     = false;
        m_errorString = "Illegal characters: " + chars;
        return true;
    }

    bool Formula::checkForUndeclaredArguments() {
        const auto undeclared = undeclaredVariables();
        if (setErrorIf(not undeclared.empty(), "Undeclared arguments: " + alg::StringAlg::setToString(undeclared))) {
            return true;
        }
        return false;
    }

    bool Formula::parseAst(const std::string& string) {
        m_ast = std::make_unique<ast::Ast>(string);
        if (setErrorIf(not m_ast->success(), m_ast->errorString())) {
            return false;
        }
        return true;
    }

    bool Formula::parseFormulaHeader(const std::string& string) {
        m_formulaHeader = std::make_unique<FormulaHeader>(string);
        if (setErrorIf(not m_formulaHeader->success(), m_formulaHeader->errorString())) {
            return false;
        }
        return true;
    }

    const std::set<std::string>& Formula::declaredVariables() const {
        return m_formulaHeader->variables();
    }

    std::set<std::string> Formula::referencedVariables() const {
        return m_ast->variables();
    }

    std::set<std::string> Formula::unusedVariables() const {
        std::set<std::string> result;
        const auto&           declared   = declaredVariables();
        const auto&           referenced = referencedVariables();

        std::set_difference(declared.begin(), declared.end(), referenced.begin(), referenced.end(), std::inserter(result, result.begin()));
        return result;
    }

    std::set<std::string> Formula::undeclaredVariables() const {
        std::set<std::string> result;
        const auto&           declared   = declaredVariables();
        const auto&           referenced = referencedVariables();

        std::set_difference(referenced.begin(), referenced.end(), declared.begin(), declared.end(), std::inserter(result, result.begin()));
        return result;
    }

    std::string Formula::getHints() const {
        const auto unused = unusedVariables();
        return unused.empty() ? "" : "Unused variables: " + alg::StringAlg::setToString(unused);
    }

    gen::Number Formula::eval(const std::map<std::string, gen::Number>& arguments) const {
        return m_ast->eval(arguments);
    }

    const std::string& Formula::toString() const {
        return m_string;
    }

    std::string Formula::toProcessedString() const {
        std::string result;
        result += m_formulaHeader->name() + ": " + alg::StringAlg::setToString(m_formulaHeader->variables()) + "\n";
        result += m_ast->toString() + "\n";

        return result;
    }
} // namespace fml
