//
// Created by pc on 21-11-20.
//

#ifndef PARSER_PATTERN_H
#define PARSER_PATTERN_H

#include "../AstNode.h"

#include <cassert>
#include <functional>
#include <map>
#include <string>

typedef std::function<bool(const AstNode*)> Predicate;

class Pattern {
  public:
    enum class PATTERN_TOKEN { CH0, CH1, CLOSE, MINUS, NAME_A, NAME_B, OR, ZERO, ONE, AND };

    explicit Pattern(std::vector<PATTERN_TOKEN> pattern);

    bool match(const AstNode* node);

    [[nodiscard]] const AstNode* node(const std::string& name) const;

  private:
    enum class BOOL_MODE { AND, OR };

    static bool combine(bool b1, bool b2, BOOL_MODE mode);

    bool match(const AstNode* node, std::map<std::string, const AstNode*>& names);

    const std::vector<PATTERN_TOKEN>      m_pattern;
    size_t                                m_index = 0;
    std::map<std::string, const AstNode*> m_names;
};

#endif // PARSER_PATTERN_H
