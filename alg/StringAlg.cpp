//
// Created by pc on 28-05-21.
//

#include "StringAlg.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace alg::str {

    std::vector<std::string> TRIM_AND_SPLIT(std::string string, const std::string& splitCharacters) {
        boost::trim(string);
        std::vector<std::string> parts;
        boost::split(parts, string, boost::is_any_of(splitCharacters));
        return parts;
    }

    std::string TRIM(std::string string) {
        boost::trim(string);
        return string;
    }

} // namespace alg::str