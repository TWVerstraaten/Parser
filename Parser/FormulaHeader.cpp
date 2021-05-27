//
// Created by pc on 09-04-21.
//

#include "FormulaHeader.h"

#include "Tokenizer.h"

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>

bool checkIdentifier(const std::string& string) {
    auto trimmedString = string;
    boost::trim(trimmedString);
    assert(trimmedString == string);

    auto it = trimmedString.begin();
    if (not isalpha(*it)) {
        return false;
    }
    ++it;
    for (; it != trimmedString.end(); ++it) {
        if (not isalnum(*it)) {
            return false;
        }
    }
    return true;
}

FormulaHeader::FormulaHeader(std::string headerString) : m_headerString(std::move(headerString)) {
    auto trimmedString = m_headerString;
    boost::trim_if(trimmedString, boost::is_any_of("\t "));

    std::vector<std::string> parts;
    boost::split(parts, trimmedString, boost::is_any_of("("));
    assert(not parts.empty());

    m_name = parts.at(0);
    boost::trim(m_name);
    assert(checkIdentifier(m_name));

    if (parts.size() == 2) {
        assert(parts.size() == 2);
        assert(parts.at(1).at(parts.at(1).length() - 1) == ')');

        boost::tokenizer<> tokenizer(parts.at(1));
        for (boost::tokenizer<>::iterator token = tokenizer.begin(); token != tokenizer.end(); ++token) {
            std::string next = *token;
            boost::trim(next);
            assert(checkIdentifier(next));
            m_variables.insert(next);
        }
    }
    std::cout << "Name =\t " << m_name << "\nVariables =\t ";
    for (const auto& var : m_variables) {
        std::cout << var << " ";
    }
    std::cout << '\n';
}

const std::string& FormulaHeader::name() const {
    return m_name;
}

const std::unordered_set<std::string>& FormulaHeader::variables() const {
    return m_variables;
}

const std::string& FormulaHeader::headerString() const {
    return m_headerString;
}
