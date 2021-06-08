//
// Created by pc on 06-06-21.
//

#include "ParserError.h"

#include <cassert>

std::string ParserError::toString() const {
    switch (m_type) {
        case TYPE::ILLEGAL_CHARACTER:
            return "Illegal Character (" + m_message + ") at " + std::to_string(m_range.m_startIndex);
        case TYPE::UNMATCHED_CLOSING_BR:
            return "Unmatched closing bracket at " + std::to_string(m_range.m_startIndex);
        case TYPE::UNMATCHED_OPEN_BR:
            return "Unmatched open bracket at " + std::to_string(m_range.m_startIndex);
        case TYPE::TOO_MANY_EQUALS:
            return "Too many equals signs, first at " + std::to_string(m_range.m_startIndex);
        case TYPE::ILLEGAL_SEQUENCE:
            return "Illegal sequence " + m_message + " at " + m_range.toString();
        case TYPE::IDENTIFIER_ERROR:
            return "Can't parse identifier " + m_message + " at " + m_range.toString();
        case TYPE::NUMBER_ERROR:
            return "Can't parse number " + m_message + " at " + m_range.toString();
    }
    assert(false);
    return "";
}
