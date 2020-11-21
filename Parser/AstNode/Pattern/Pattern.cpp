//
// Created by pc on 21-11-20.
//

#include "Pattern.h"

#include <iostream>
#include <utility>

Pattern::Pattern(std::vector<PATTERN_TOKEN> pattern) : m_pattern(std::move(pattern)) {
}

bool Pattern::match(const AstNode* node) {
    return match(node, m_names);
}

bool Pattern::match(const AstNode* node, std::map<std::string, const AstNode*>& names) {
    bool      matchedSoFar = true;
    BOOL_MODE mode         = BOOL_MODE::AND;
    if (m_pattern.at(m_index) == PATTERN_TOKEN::OR) {
        mode = BOOL_MODE::OR;
    }
    while (m_pattern.at(m_index) != PATTERN_TOKEN::CLOSE) {
        switch (m_pattern.at(m_index)) {
            case PATTERN_TOKEN::CLOSE:
                return matchedSoFar;
            case PATTERN_TOKEN::MINUS:
                matchedSoFar = combine(node->type() == AstNode::NODE_TYPE::UNARY_MINUS, matchedSoFar, mode);
                break;
            case PATTERN_TOKEN::NAME_A:
                if (names.find("A") == names.end()) {
                    names.insert({"A", node});
                } else {
                    matchedSoFar = combine((*names["A"]) == *node, matchedSoFar, mode);
                }
                break;
            case PATTERN_TOKEN::NAME_B:
                if (names.find("B") == names.end()) {
                    names.insert({"B", node});
                } else {
                    matchedSoFar = combine((*names["B"]) == *node, matchedSoFar, mode);
                }
                break;
            case PATTERN_TOKEN::ZERO:
                if (not node->isNumeric()) {
                    return false;
                }
                matchedSoFar = combine(matchedSoFar, AstNode::doubleValue(node) == 0, mode);
            case PATTERN_TOKEN::ONE:
                break;

            case PATTERN_TOKEN::CH0:
                if (node->childCount() == 0) {
                    return false;
                }
                ++m_index;
                matchedSoFar = match(node->childAt(0), names);
                break;
            case PATTERN_TOKEN::OR:
                break;
            case PATTERN_TOKEN::AND:
                break;
            case PATTERN_TOKEN::CH1:
                if (node->childCount() < 2) {
                    return false;
                }
                ++m_index;
                matchedSoFar = match(node->childAt(1), names);
                break;
        }
        ++m_index;
    }
    return matchedSoFar;
}

bool Pattern::combine(bool b1, bool b2, Pattern::BOOL_MODE mode) {
    return mode == BOOL_MODE::AND ? (b1 && b2) : (b1 || b2);
}

const AstNode* Pattern::node(const std::string& name) const {
    if (m_names.find(name) == m_names.end()) {
        return nullptr;
    } else {
        return m_names.at(name);
    }
}
