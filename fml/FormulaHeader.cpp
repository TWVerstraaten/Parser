//
// Created by pc on 09-04-21.
//

#include "FormulaHeader.h"

#include "../alg/StringAlg.h"
#include "prs/Tokenizer.h"

#include <boost/tokenizer.hpp>
#include <set>

namespace fml {
    static std::set<char> findIllegalCharactersInHeader(const std::string& string) {
        std::set<char>           illegalCharacters;
        static const std::string allowedSpecialCharacters = ",()\t ";
        for (char c : string) {
            if (not isalnum(c) && allowedSpecialCharacters.find_first_of(c) == std::string::npos) {
                illegalCharacters.insert(c);
            }
        }
        return illegalCharacters;
    }

    static bool checkIdentifier(const std::string& string) {
        auto trimmedString = alg::StringAlg::S_TRIM(string);
        assert(trimmedString == string);
        if (trimmedString.empty()) {
            return false;
        }
        auto it = trimmedString.begin();
        if (not isalpha(*it)) {
            return false;
        }
        ++it;
        return std::all_of(it, trimmedString.end(), isalnum);
    }

    FormulaHeader::FormulaHeader(std::string headerString) : m_headerString(std::move(headerString)) {
        if ((containsIllegalCharacters())) {
            return;
        }
        const auto parts = alg::StringAlg::S_TRIM_AND_SPLIT(m_headerString, "(");
        if (setErrorIf(parts.empty(), "Empty function headerVariant") || setErrorIf(parts.size() > 2, "Too many opening brackets")) {
            return;
        }
        m_name = alg::StringAlg::S_TRIM(parts.at(0));
        assert(m_name == alg::StringAlg::S_TRIM(m_name));

        if (setErrorIf(not checkIdentifier(m_name), "Invalid function name: " + m_name)) {
            return;
        }
        if (parts.size() == 2) {
            parseArguments(parts.at(1));
        }
    }

    void FormulaHeader::parseArguments(const std::string& argumentString) {
        if (setErrorIf(argumentString.empty(), "Empty argument list")) {
            return;
        }
        if (setErrorIf(argumentString.at(argumentString.length() - 1) != ')', "Closing bracket should be last character of headerVariant")) {
            return;
        }

        boost::tokenizer<>    tokenizer(argumentString);
        std::set<std::string> declaredVariableSet;
        std::set<std::string> duplicateVariableSet;
        for (boost::tokenizer<>::iterator token = tokenizer.begin(); token != tokenizer.end(); ++token) {
            std::string next = alg::StringAlg::S_TRIM(*token);
            if (setErrorIf(not checkIdentifier(next), "Invalid variable name: " + next)) {
                return;
            }
            m_variables.push_back(next);
            if (declaredVariableSet.find(next) != declaredVariableSet.end()) {
                duplicateVariableSet.insert(next);
            } else {
                declaredVariableSet.insert(next);
            }
        }
        if (not duplicateVariableSet.empty()) {
            m_success     = false;
            m_errorString = "Duplicate variables: " + alg::StringAlg::S_CONCATENATE_STRINGS(duplicateVariableSet);
        }
    }

    const std::string& FormulaHeader::name() const {
        return m_name;
    }

    const std::vector<std::string>& FormulaHeader::variables() const {
        return m_variables;
    }

    const std::string& FormulaHeader::headerString() const {
        return m_headerString;
    }

    bool FormulaHeader::containsIllegalCharacters() {
        auto illegalCharacters = findIllegalCharactersInHeader(m_headerString);
        if (illegalCharacters.empty()) {
            return false;
        }
        std::string chars;
        std::for_each(illegalCharacters.begin(), illegalCharacters.end(), [&](char c) { chars += c; });
        m_success     = false;
        m_errorString = "Illegal characters in headerVariant: " + chars;
        return true;
    }
} // namespace fml