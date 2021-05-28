//
// Created by pc on 28-05-21.
//

#include "StringAlg.h"

std::string StringAlg::setToString(const std::set<std::string>& set) {
    std::ostringstream stream;
    std::copy(set.begin(), set.end(), std::ostream_iterator<std::string>(stream, ","));
    std::string result = stream.str();
    return result.empty() ? "" : result.substr(0, result.length() - 1);
}
