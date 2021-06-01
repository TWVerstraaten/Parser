//
// Created by pc on 28-05-21.
//

#include "BoostWrapper.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace alg {

    std::vector<std::string> BoostWrapper::trimAndSplit(std::string string, const std::string& splitCharacters) {
        boost::trim(string);
        std::vector<std::string> parts;
        boost::split(parts, string, boost::is_any_of(splitCharacters));
        return parts;
    }

    std::string BoostWrapper::trim(std::string string) {
        boost::trim(string);
        return string;
    }
} // namespace alg
