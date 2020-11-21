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

class Pattern {
  public:
    enum class PATTERN_TOKEN { CH0, CH1, CLOSE, MINUS, NAME_A, NAME_B, OR, ZERO, ONE, AND, NUM };

    explicit Pattern(std::vector<PATTERN_TOKEN> pattern, bool testCommutative);

    static Pattern oneChildIs(Pattern::PATTERN_TOKEN token, bool testCommutative);
    static Pattern childrenEqual();

    bool                         match(const AstNode* node);
    [[nodiscard]] const AstNode* node(const std::string& name) const;
    void                         reset();

  private:
    enum class BOOL_MODE { AND, OR };
    static bool combine(bool b1, bool b2, BOOL_MODE mode);
    bool        matchInternal(const AstNode* node);

    const std::vector<PATTERN_TOKEN>      m_pattern;
    size_t                                m_index = 0;
    std::map<std::string, const AstNode*> m_names;
    bool                                  m_testCommutative;
    bool                                  m_childIndicesWereSwapped = false;
};

#endif // PARSER_PATTERN_H
