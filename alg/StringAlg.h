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
        template <class Container>
        static std::string setToString(const Container& set) {
            std::ostringstream stream;
            std::copy(set.begin(), set.end(), std::ostream_iterator<std::string>(stream, ","));
            std::string result = stream.str();
            return result.empty() ? "" : result.substr(0, result.length() - 1);
        }
    };
} // namespace alg

#endif // PARSER_STRINGALG_H
