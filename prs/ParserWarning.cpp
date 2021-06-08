//
// Created by pc on 07-06-21.
//

#include "ParserWarning.h"

#include <cassert>

std::string ParserWarning::toString() const {
    switch (m_type) {
        case SUSPICIOUS_IDENTIFIER_NUM_PATTERN:
            return "Suspicious pattern: will be parsed as " + m_message + " at " + m_range.toString();
    }

    assert(false);
    return "";
}
