//
// Created by pc on 07-06-21.
//

#ifndef PRS_RANGE_H
#define PRS_RANGE_H

#include <string>

class Range {

  public:
    Range(size_t startIndex, size_t endIndex);
    Range() = default;

    [[nodiscard]] bool        isEmpty() const;
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] size_t      startIndex() const;
    [[nodiscard]] size_t      endIndex() const;

  private:
    size_t m_startIndex = std::numeric_limits<size_t>::max();
    size_t m_endIndex   = std::numeric_limits<size_t>::max();
};

#endif // PRS_RANGE_H
