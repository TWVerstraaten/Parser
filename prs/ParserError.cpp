//
// Created by pc on 06-06-21.
//

#include "ParserError.h"

#include <cassert>

ParserError::ParserError(ParserError::ERROR_TYPE type, std::string message, size_t startIndexInString, size_t endIndexInString)
    : m_type(type), m_message(std::move(message)), m_startIndexInString(startIndexInString), m_endIndexInString(endIndexInString) {
}

std::string ParserError::toString() const {
    switch (m_type) {
        case ERROR_TYPE::ILLEGAL_CHARACTER:
            return "Illegal Character (" + m_message + ") at " + std::to_string(m_startIndexInString);
        case ERROR_TYPE::UNMATCHED_CLOSING_BR:
            return "Unmatched closing bracket at " + std::to_string(m_startIndexInString);
        case ERROR_TYPE::UNMATCHED_OPEN_BR:
            return "Unmatched open bracket at " + std::to_string(m_startIndexInString);
        case ERROR_TYPE::TOO_MANY_EQUALS:
            return "Too many equals signs, first at " + std::to_string(m_startIndexInString);
        case ERROR_TYPE::ILLEGAL_SEQUENCE:
            return "Illegal sequence " + m_message + " from " + std::to_string(m_startIndexInString) + " to " +
                   std::to_string(m_endIndexInString);
    }
    assert(false);
    return "";
}
