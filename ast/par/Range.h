//
// Created by pc on 07-06-21.
//

#ifndef AST_PAR_RANGE_H
#define AST_PAR_RANGE_H

#include <string>

namespace ast::par {
    class Range {

      public:
        Range(size_t startIndex, size_t endIndex);
        Range() = default;

        [[nodiscard]] bool        isEmpty() const;
        [[nodiscard]] bool        isInRange(size_t position) const;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] size_t      startIndex() const;
        [[nodiscard]] size_t      endIndex() const;

      private:
        size_t m_startIndex = std::numeric_limits<size_t>::max();
        size_t m_endIndex   = std::numeric_limits<size_t>::max();
    };
} // namespace ast::par

#endif // AST_PAR_RANGE_H
