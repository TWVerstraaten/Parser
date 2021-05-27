//
// Created by pc on 27-05-21.
//

#include "Formula.h"

#include <boost/algorithm/string.hpp>

Formula::Formula(std::string string) : m_string(std::move(string)) {
    std::string trimmedString = m_string;
    boost::trim(trimmedString);
    std::vector<std::string> parts;
    boost::split(parts, trimmedString, boost::is_any_of("="));
    if (parts.size() == 1) {
        m_parseSuccessful = false;
        m_errorString     = "Does not contain a '=' sign";
    }
    if (parts.size() > 2) {
        m_parseSuccessful = false;
        m_errorString     = "Too many '=' signs";
    }
    m_ast = std::make_unique<Ast>(parts.at(1));
    if (not m_ast->parseSuccessful()) {
        m_parseSuccessful = false;
        m_errorString     = m_ast->errorString();
    }
}

bool Formula::parseSuccessful() const {
    return m_parseSuccessful;
}

const std::string& Formula::errorString() const {
    return m_errorString;
}
