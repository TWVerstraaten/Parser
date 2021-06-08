//
// Created by pc on 07-06-21.
//

#include "Range.h"

#include <cassert>

Range::Range(size_t startIndex, size_t endIndex) : m_startIndex(startIndex), m_endIndex(endIndex) {
}

std::string Range::toString() const {
    if (m_startIndex == std::numeric_limits<size_t>::max()) {
        assert(m_endIndex == std::numeric_limits<size_t>::max());
        return "[Empty range]";
    } else {
        return "[" + std::to_string(m_startIndex) + "," + std::to_string(m_endIndex) + "]";
    }
}

size_t Range::startIndex() const {
    return m_startIndex;
}

size_t Range::endIndex() const {
    return m_endIndex;
}
