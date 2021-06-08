//
// Created by pc on 07-06-21.
//

#ifndef PARSER_RANGE_H
#define PARSER_RANGE_H

#include <cstddef>
#include <string>

struct Range {

    [[nodiscard]] std::string toString() const;

    size_t m_startIndex;
    size_t m_endIndex;
};

#endif // PARSER_RANGE_H
