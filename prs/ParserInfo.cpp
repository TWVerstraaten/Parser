//
// Created by pc on 07-06-21.
//

#include "ParserInfo.h"

void ParserInfo::addError(ParserError&& error) {
    m_errors.emplace_back(std::move(error));
}

void ParserInfo::addWarning(ParserWarning&& warning) {
    m_warnings.emplace_back(std::move(warning));
}

bool ParserInfo::success() const {
    return m_errors.empty();
}

bool ParserInfo::hasWarning() const {
    return not m_warnings.empty();
}

const std::vector<ParserError>& ParserInfo::errors() const {
    return m_errors;
}

const std::vector<ParserWarning>& ParserInfo::warnings() const {
    return m_warnings;
}
