//
// Created by pc on 21-11-20.
//

#ifndef PARSER_PATTERN_H
#define PARSER_PATTERN_H

#include "../AstNode.h"

#include <map>
#include <string>

class Pattern {
  public:




  private:
    std::map<std::string, const AstNode*> m_names;
};

#endif // PARSER_PATTERN_H
