//
// Created by pc on 06-06-21.
//

#include "Tokenizer.h"

#include "../gen/defines.h"
#include "ParserInfo.h"
#include "StructuralToken.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

Tokenizer::Tokenizer(std::string string, ParserInfo& info) : m_string(std::move(string)), m_info(info) {
    findIllegalCharacters();
    tokenize();
    if (not m_info.success()) {
        return;
    }
    checkBrackets();
    checkDoubleEquals();
    checkRepeatedCommas();
    checkRepeatedOperators();
    checkIdentifierNumberPatternWithNoSpace();
}

const std::list<Token>& Tokenizer::tokenList() const {
    return m_tokenList;
}

void Tokenizer::findIllegalCharacters() {
    static const std::string allowedSpecialCharacters = ".,()+-*/^=\t ";
    for (size_t i = 0; i != m_string.size(); ++i) {
        const char c = m_string.at(i);
        if (not isalnum(c) && allowedSpecialCharacters.find_first_of(c) == std::string::npos) {
            m_info.addError({ParserError::ERROR_TYPE::ILLEGAL_CHARACTER, std::string(1, c), {i, i}});
        }
    }
}

void Tokenizer::tokenize() {
    if (not m_info.success()) {
        return;
    }
    for (size_t i = 0; i != m_string.size(); ++i) {
        const char c = m_string.at(i);
        switch (c) {
            case '(':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::LEFT_BR, "(", {i, i}});
                break;
            case ')':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::RIGHT_BR, ")", {i, i}});
                break;
            case '+':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::PLUS, "+", {i, i}});
                break;
            case '-':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::MINUS, "-", {i, i}});
                break;
            case '*':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::TIMES, "*", {i, i}});
                break;
            case '/':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::DIVIDE, "/", {i, i}});
                break;
            case '^':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::POWER, "^", {i, i}});
                break;
            case ',':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::COMMA, ",", {i, i}});
                break;
            case '=':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::EQUALS, "=", {i, i}});
                break;
            default:
                if (isalpha(c)) {
                    size_t j = i + 1;
                    while (j < m_string.size() && isalnum(m_string.at(j))) {
                        ++j;
                    }
                    m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::IDENTIFIER, m_string.substr(i, j - i), {i, j - 1}});
                    i = j - 1;
                } else if (isdigit(c) || c == '.') {
                    size_t j = i + 1;
                    while (j < m_string.size() && (isdigit(m_string.at(j)) || m_string.at(j) == '.')) {
                        ++j;
                    }
                    m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::NUMBER, m_string.substr(i, j - i), {i, j - 1}});
                    i = j - 1;
                } else {
                    if (not isspace(c)) {
                        std::cout << "Cannot tokenize " << c << '\n';
                    }
                }
                break;
        }
    }
    replaceUnaryMinuses();
}

void Tokenizer::replaceUnaryMinuses() {
    bool nextMinusIsUnary = true;
    for (auto& token : m_tokenList) {
        if (token.m_type == Token::TOKEN_TYPE::MINUS && nextMinusIsUnary) {
            token.m_type = Token::TOKEN_TYPE::UNARY_MINUS;
        } else
            switch (token.m_type) {
                case Token::TOKEN_TYPE::LEFT_BR:
                    [[fallthrough]];
                case Token::TOKEN_TYPE::EQUALS:
                    nextMinusIsUnary = true;
                    break;
                default:
                    nextMinusIsUnary = false;
                    break;
            }
    }
}

std::string Tokenizer::toString(bool readable) const {
    std::stringstream ss;
    for (const auto& token : m_tokenList) {
        ss << token.toString(readable);
    }
    ss << '\n';
    return ss.str();
}

void Tokenizer::checkBrackets() {
    size_t bracketDepth = 0;
    for (auto& token : m_tokenList) {
        if (token.m_type == Token::TOKEN_TYPE::LEFT_BR) {
            ++bracketDepth;
        } else if (token.m_type == Token::TOKEN_TYPE::RIGHT_BR) {
            if (bracketDepth == 0) {
                m_info.addError({ParserError::ERROR_TYPE::UNMATCHED_CLOSING_BR, "", token.m_range});
                return;
            }
            --bracketDepth;
        }
    }
    if (bracketDepth != 0) {
        auto it = std::find_if(m_tokenList.rbegin(), m_tokenList.rend(), TT_LAMBDA(a, return a.m_type == Token::TOKEN_TYPE::LEFT_BR;));
        assert(it != m_tokenList.rend());
        m_info.addError({ParserError::ERROR_TYPE::UNMATCHED_OPEN_BR, "", it->m_range});
    }
}

void Tokenizer::checkDoubleEquals() {
    auto it = std::find_if(TT_IT(m_tokenList), TT_LAMBDA(a, return a.m_type == Token::TOKEN_TYPE::EQUALS;));
    if (it == m_tokenList.end()) {
        return;
    }
    it = std::find_if(std::next(it), m_tokenList.end(), TT_LAMBDA(a, return a.m_type == Token::TOKEN_TYPE::EQUALS;));
    if (it != m_tokenList.end()) {
        m_info.addError({ParserError::ERROR_TYPE::TOO_MANY_EQUALS, "", it->m_range});
    }
}

void Tokenizer::checkRepeatedOperators() {
    const std::set<Token::TOKEN_TYPE> operatorTypes{Token::TOKEN_TYPE::POWER,
                                                    Token::TOKEN_TYPE::PLUS,
                                                    Token::TOKEN_TYPE::MINUS,
                                                    Token::TOKEN_TYPE::TIMES,
                                                    Token::TOKEN_TYPE::DIVIDE,
                                                    Token::TOKEN_TYPE::UNARY_MINUS};
    const std::set<Token::TOKEN_TYPE> allowedAfterOperator = {
        Token::TOKEN_TYPE::IDENTIFIER, Token::TOKEN_TYPE::NUMBER, Token::TOKEN_TYPE::LEFT_BR};

    for (auto it = m_tokenList.begin(); std::next(it) != m_tokenList.end(); ++it) {
        const auto currentType = it->m_type;
        if (operatorTypes.find(currentType) != operatorTypes.end()) {
            const auto nextType = std::next(it)->m_type;
            if (allowedAfterOperator.find(nextType) == allowedAfterOperator.end()) {
                m_info.addError({ParserError::ERROR_TYPE::ILLEGAL_SEQUENCE,
                                 it->m_string + " " + std::next(it)->m_string,
                                 {it->m_range.m_startIndex, std::next(it)->m_range.m_endIndex}});
            }
        }
    }
}

void Tokenizer::checkRepeatedCommas() {
    bool lastWasComma = false;
    for (auto it = m_tokenList.begin(); it != m_tokenList.end(); ++it) {
        if (it->m_type == Token::TOKEN_TYPE::COMMA) {
            if (lastWasComma) {
                assert(it != m_tokenList.begin());
                m_info.addError({ParserError::ERROR_TYPE::ILLEGAL_SEQUENCE,
                                 ", ,",
                                 {std::prev(it)->m_range.m_startIndex, it->m_range.m_endIndex}});
            } else {
                lastWasComma = true;
            }
        } else {
            lastWasComma = false;
        }
    }
}

void Tokenizer::checkIdentifierNumberPatternWithNoSpace() {
    if (m_tokenList.size() < 2) {
        return;
    }
    for (auto it = m_tokenList.begin(); std::next(it) != m_tokenList.end(); ++it) {
        auto next = std::next(it);
        if (it->m_type == Token::TOKEN_TYPE::IDENTIFIER && next->m_type == Token::TOKEN_TYPE::NUMBER) {
            if (it->m_range.m_endIndex + 1 == next->m_range.m_startIndex) {
                m_info.addWarning({ParserWarning::WARNING_TYPE::SUSPICIOUS_IDENTIFIER_NUM_PATTERN,
                                   it->m_string + " * " + next->toString(),
                                   {it->m_range.m_startIndex, next->m_range.m_endIndex}});
            }
        }
    }
}
