//
// Created by pc on 21-11-20.
//

#include "Pattern.h"

#include <iostream>
#include <utility>

Pattern::Pattern(std::vector<PATTERN_TOKEN> pattern, bool testCommutative)
    : m_pattern(std::move(pattern)), m_testCommutative(testCommutative) {
}

bool Pattern::matchInternal(const AstNode* node) {
    bool      matchedSoFar = true;
    BOOL_MODE mode         = BOOL_MODE::AND;
    if (m_pattern.at(m_index) == PATTERN_TOKEN::OR) {
        mode = BOOL_MODE::OR;
    }
    while (m_pattern.at(m_index) != PATTERN_TOKEN::CLOSE) {
        assert(m_index < m_pattern.size());
        switch (m_pattern.at(m_index)) {
            case PATTERN_TOKEN::CLOSE:
                return matchedSoFar;
            case PATTERN_TOKEN::MINUS:
                matchedSoFar = combine(node->type() == AstNode::NODE_TYPE::UNARY_MINUS, matchedSoFar, mode);
                break;
            case PATTERN_TOKEN::NAME_A:
                if (m_names.find("A") == m_names.end()) {
                    m_names.insert({"A", node});
                } else {
                    matchedSoFar = combine((*m_names["A"]) == *node, matchedSoFar, mode);
                }
                break;
            case PATTERN_TOKEN::NAME_B:
                if (m_names.find("B") == m_names.end()) {
                    m_names.insert({"B", node});
                } else {
                    matchedSoFar = combine((*m_names["B"]) == *node, matchedSoFar, mode);
                }
                break;
            case PATTERN_TOKEN::ZERO:
                if (not node->isNumeric()) {
                    return false;
                }
                matchedSoFar = combine(matchedSoFar, AstNode::doubleValue(node) == 0, mode);
                break;
            case PATTERN_TOKEN::ONE:
                if (not node->isNumeric()) {
                    return false;
                }
                matchedSoFar = combine(matchedSoFar, AstNode::doubleValue(node) == 1, mode);
                break;
            case PATTERN_TOKEN::CH0:
                if (node->childCount() == 0) {
                    return false;
                }
                ++m_index;
                matchedSoFar = combine(match(node->childAt(m_childIndicesWereSwapped)), matchedSoFar, mode);
                break;
            case PATTERN_TOKEN::AND:
                break;
            case PATTERN_TOKEN::CH1:
                if (node->childCount() < 2) {
                    return false;
                }
                ++m_index;
                matchedSoFar = combine(match(node->childAt(not m_childIndicesWereSwapped)), matchedSoFar, mode);
                break;
            case PATTERN_TOKEN::NUM:
                matchedSoFar = combine(matchedSoFar, node->isNumeric(), mode);
                break;
            default:
                break;
        }
        ++m_index;
    }
    return matchedSoFar;
}

bool Pattern::match(const AstNode* node) {
    bool test = matchInternal(node);
    if (not m_testCommutative || test) {
        return test;
    } else {
        reset();
        m_childIndicesWereSwapped = true;
        return matchInternal(node);
    }
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

void Pattern::reset() {
    m_index = 0ul;
    m_names.clear();
}

Pattern Pattern::oneChildIs(Pattern::PATTERN_TOKEN token, bool testCommutative) {
    using T = Pattern::PATTERN_TOKEN;
    return Pattern({T::CH0, token, T::CLOSE, T::CH1, T::NAME_A, T::CLOSE, T::CLOSE}, testCommutative);
}

Pattern Pattern::childrenEqual() {
    using T = Pattern::PATTERN_TOKEN;
    return Pattern({T::CH0, T::NAME_A, T::CLOSE, T::CH1, T::NAME_A, T::CLOSE, T::CLOSE}, false);
}
