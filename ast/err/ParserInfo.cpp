//
// Created by pc on 07-06-21.
//

#include "ParserInfo.h"

#include <iostream>

namespace ast::err {

    ParserInfo::ParserInfo() = default;

    ParserInfo::~ParserInfo() = default;

    void ParserInfo::add(ParserError&& error) {
        m_parserErrors.emplace_back(std::move(error));
    }

    void ParserInfo::add(ParserWarning&& warning) {
        m_warnings.emplace_back(std::move(warning));
    }

    void ParserInfo::add(ParserMessage&& warning) {
        m_messages.emplace_back(std::move(warning));
    }

    bool ParserInfo::success() const {
        return not hasErrors();
    }

    bool ParserInfo::hasErrors() const {
        return not m_parserErrors.empty();
    }

    bool ParserInfo::hasWarnings() const {
        return not m_warnings.empty();
    }

    bool ParserInfo::hasMessages() const {
        return not m_messages.empty();
    }

    const std::vector<ParserError>& ParserInfo::parserErrors() const {
        return m_parserErrors;
    }

    const std::vector<ParserWarning>& ParserInfo::warnings() const {
        return m_warnings;
    }

    const std::vector<ParserMessage>& ParserInfo::messages() const {
        return m_messages;
    }

    void ParserInfo::printAll() const {
        std::cout << "Error count \t" << m_parserErrors.size() << '\n';
        if (hasErrors()) {
            for (const auto& el : m_parserErrors) {
                std::cout << std::string(5, '-') << el.toString() << '\n';
            }
        }
        std::cout << "Warning count \t" << m_warnings.size() << '\n';
        if (hasWarnings()) {
            for (const auto& el : m_warnings) {
                std::cout << std::string(5, '-') << el.toString() << '\n';
            }
        }
        std::cout << "Message count \t" << m_messages.size() << '\n';
        if (hasMessages()) {
            for (const auto& el : m_messages) {
                std::cout << std::string(5, '-') << el.toString() << '\n';
            }
        }
    }

    void ParserInfo::clear() {
        m_parserErrors.clear();
        m_warnings.clear();
        m_messages.clear();
    }

} // namespace ast::err