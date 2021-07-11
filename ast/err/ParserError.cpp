//
// Created by pc on 06-06-21.
//

#include "ParserError.h"

#include <cassert>
#include <utility>

namespace ast::err {
    ParserError::ParserError(TYPE type, std::string message, par::Range range) : m_type(type), m_message(std::move(message)), m_range(range) {
    }

    std::string ParserError::toString() const {
        switch (m_type) {
            case TYPE::ILLEGAL_CHARACTER:
                return "Illegal Character (" + m_message + ") at " + std::to_string(m_range.startIndex());
            case TYPE::UNMATCHED_CLOSING_BR:
                return "Unmatched closing bracket at " + std::to_string(m_range.startIndex());
            case TYPE::UNMATCHED_OPEN_BR:
                return "Unmatched open bracket at " + std::to_string(m_range.startIndex());
            case TYPE::TOO_MANY_EQUALS:
                return "Too many equals signs, first repeated at " + std::to_string(m_range.startIndex());
            case TYPE::ILLEGAL_SEQUENCE:
                return "Illegal sequence " + m_message + " at " + m_range.toString();
            case TYPE::IDENTIFIER_ERROR:
                return "Can't parse identifier " + m_message + " at " + m_range.toString();
            case TYPE::NUMBER_ERROR:
                return "Can't parse number " + m_message + " at " + m_range.toString();
            case TYPE::WRONG_ARGUMENT_COUNT_RESERVED:
                return "Wrong argument count " + m_message + " at " + m_range.toString();
            case TYPE::GENERIC:
                return "Generic Parser Error: " + m_message + " at " + m_range.toString();
            case TYPE::UNFINISHED:
                return "Unfinished Formula: naked ending " + m_message + " at " + m_range.toString();
            case TYPE::NOT_AN_ARGUMENT:
                return "Not an argument: " + m_message + " at " + m_range.toString();
            case TYPE::EMPTY_ARGUMENT:
                return "Empty argument: at " + m_range.toString();
            case TYPE::NO_ARGUMENTS:
                return "Constant should have no argument brackets: at " + m_range.toString();
            case TYPE::REPEATED_ARGUMENT:
                return "Repeated argument: " + m_message + " at " + m_range.toString();
        }
        assert(false);
    }

    const par::Range& ParserError::range() const {
        return m_range;
    }
} // namespace ast::err