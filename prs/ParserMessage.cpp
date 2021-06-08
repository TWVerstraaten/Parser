//
// Created by pc on 08-06-21.
//

#include "ParserMessage.h"

#include <cassert>

std::string ParserMessage::toString() const {
    switch (m_type) {
        case INSERT_MULTIPLICATION:
            return "Inserting '*' at " + std::to_string(m_range.m_startIndex);
    }
    assert(false);
    return "";
}
