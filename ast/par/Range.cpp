//
// Created by pc on 07-06-21.
//

#include "Range.h"

namespace ast::par {

    Range::Range(size_t startIndex, size_t endIndex) : m_startIndex(startIndex), m_endIndex(endIndex) {
    }

    std::string Range::toString() const {
        if (m_startIndex == std::numeric_limits<size_t>::max() && m_endIndex == std::numeric_limits<size_t>::max()) {
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

    bool Range::isEmpty() const {
        return m_endIndex == std::numeric_limits<size_t>::max() && m_startIndex == std::numeric_limits<size_t>::max();
    }

    bool Range::isInRange(size_t position) const {
        return m_startIndex <= position && position <= m_endIndex;
    }
} // namespace ast::par