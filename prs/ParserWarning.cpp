//
// Created by pc on 07-06-21.
//

#include "ParserWarning.h"

#include <cassert>

ParserWarning::ParserWarning(ParserWarning::WARNING_TYPE type, std::string message, size_t startIndexInString, size_t endIndexInString)
    : m_type(type), m_message(std::move(message)), m_startIndexInString(startIndexInString), m_endIndexInString(endIndexInString) {
}

std::string ParserWarning::toString() const {
    switch (m_type) {
        case SUSPICIOUS_IDENTIFIER_NUM_PATTERN:
            return "Suspicious pattern: will be parsed as " + m_message + " [" + std::to_string(m_startIndexInString) + "," +
                   std::to_string(m_endIndexInString) + "]";
    }

    assert(false);
    return "";
}
