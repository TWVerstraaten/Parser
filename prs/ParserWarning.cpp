//
// Created by pc on 07-06-21.
//

#include "ParserWarning.h"

#include <cassert>

ParserWarning::ParserWarning(TYPE type, const std::string& message, Range range)
    : m_type(type), m_message(message), m_range(range) {
}

std::string ParserWarning::toString() const {
    switch (m_type) {
        case SUSPICIOUS_IDENTIFIER_NUM_PATTERN:
            return "Suspicious pattern: will be parsed as " + m_message + " at " + m_range.toString();
    }

    assert(false);
    return "";
}
