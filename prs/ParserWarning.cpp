//
// Created by pc on 07-06-21.
//

#include "ParserWarning.h"

#include <cassert>
#include <utility>

ParserWarning::ParserWarning(TYPE type, std::string  message, Range range)
    : m_type(type), m_message(std::move(message)), m_range(range) {
}

std::string ParserWarning::toString() const {
    switch (m_type) {
        case SUSPICIOUS_IDENTIFIER_NUM_PATTERN:
            return "Suspicious pattern: will be parsed as " + m_message + " at " + m_range.toString();
    }

    assert(false);
    return "";
}
