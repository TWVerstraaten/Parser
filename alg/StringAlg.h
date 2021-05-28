//
// Created by pc on 28-05-21.
//

#ifndef PARSER_STRINGALG_H
#define PARSER_STRINGALG_H

#include <iterator>
#include <set>
#include <sstream>
#include <string>

namespace alg {
    class StringAlg {

      public:
        static std::string setToString(const std::set<std::string>& set);
    };
} // namespace alg

#endif // PARSER_STRINGALG_H
