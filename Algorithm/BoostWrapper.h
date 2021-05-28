//
// Created by pc on 28-05-21.
//

#ifndef PARSER_BOOSTWRAPPER_H
#define PARSER_BOOSTWRAPPER_H

#include <string>
#include <vector>

class BoostWrapper {
  public:
    static std::vector<std::string> trimAndSplit(std::string string, const std::string& splitCharacters);
    static std::string trim(std::string string);
};

#endif // PARSER_BOOSTWRAPPER_H
