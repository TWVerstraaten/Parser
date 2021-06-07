//
// Created by pc on 06-06-21.
//

#include "Tokenizer.h"

#include "../gen/defines.h"
#include "StructuralToken.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

Tokenizer::Tokenizer(std::string string) : m_string(std::move(string)) {
    m_errors.clear();
    findIllegalCharacters();
    tokenize();
    if (not m_success) {
        return;
    }
    checkBrackets();
    checkDoubleEquals();
    checkRepeatedCommas();
    checkRepeatedOperators();

    initializeStructuralTokens();
    extractFunctionsAndBracketsFromStructuralTokens();
}

const std::list<Token>& Tokenizer::tokenList() const {
    return m_tokenList;
}

void Tokenizer::findIllegalCharacters() {
    static const std::string allowedSpecialCharacters = ".,()+-*/^=\t ";
    for (size_t i = 0; i != m_string.size(); ++i) {
        const char c = m_string.at(i);
        if (not isalnum(c) && allowedSpecialCharacters.find_first_of(c) == std::string::npos) {
            m_errors.emplace_back(ParserError::ERROR_TYPE::ILLEGAL_CHARACTER, std::string(1, c), i, i);
        }
    }
    if (not m_errors.empty()) {
        m_success = false;
    }
}

void Tokenizer::tokenize() {
    if (not m_success) {
        return;
    }
    for (size_t i = 0; i != m_string.size(); ++i) {
        const char c = m_string.at(i);
        switch (c) {
            case '(':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::LEFT_BR, "(", i, i});
                break;
            case ')':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::RIGHT_BR, ")", i, i});
                break;
            case '+':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::PLUS, "+", i, i});
                break;
            case '-':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::MINUS, "-", i, i});
                break;
            case '*':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::TIMES, "*", i, i});
                break;
            case '/':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::DIVIDE, "/", i, i});
                break;
            case '^':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::POWER, "^", i, i});
                break;
            case ',':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::COMMA, ",", i, i});
                break;
            case '=':
                m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::EQUALS, "=", i, i});
                break;
            default:
                if (isalpha(c)) {
                    size_t j = i + 1;
                    while (j < m_string.size() && isalnum(m_string.at(j))) {
                        ++j;
                    }
                    m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::IDENTIFIER, m_string.substr(i, j - i), i, j});
                    i = j - 1;
                } else if (isdigit(c) || c == '.') {
                    size_t j = i + 1;
                    while (j < m_string.size() && (isdigit(m_string.at(j)) || m_string.at(j) == '.')) {
                        ++j;
                    }
                    m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::NUM, m_string.substr(i, j - i), i, j});
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
                    // [[fallthrough]]
                case Token::TOKEN_TYPE::EQUALS:
                    nextMinusIsUnary = true;
                    break;
                default:
                    nextMinusIsUnary = false;
                    break;
            }
    }
}

std::string Tokenizer::tokensToString(bool readable) const {
    std::stringstream ss;
    ss << "Success: \t" << m_success << '\n';
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
            token.setAdditional(bracketDepth);
            ++bracketDepth;
        } else if (token.m_type == Token::TOKEN_TYPE::RIGHT_BR) {
            if (bracketDepth == 0) {
                m_success = false;
                m_errors.emplace_back(
                    ParserError{ParserError::ERROR_TYPE::UNMATCHED_CLOSING_BR, "", token.m_startIndexInString, token.m_endIndexInString});
                return;
            }
            --bracketDepth;
            token.setAdditional(bracketDepth);
        }
    }
    if (bracketDepth != 0) {
        auto it = std::find_if(m_tokenList.rbegin(), m_tokenList.rend(), TT_LAMBDA(a, return a.m_type == Token::TOKEN_TYPE::LEFT_BR;));
        assert(it != m_tokenList.rend());
        m_success = false;
        m_errors.emplace_back(
            ParserError{ParserError::ERROR_TYPE::UNMATCHED_OPEN_BR, "", it->m_startIndexInString, it->m_endIndexInString});
    }
}

const std::vector<ParserError>& Tokenizer::errors() const {
    return m_errors;
}

bool Tokenizer::success() const {
    return m_success;
}

const std::string& Tokenizer::string() const {
    return m_string;
}

void Tokenizer::checkDoubleEquals() {
    auto it = std::find_if(TT_IT(m_tokenList), TT_LAMBDA(a, return a.m_type == Token::TOKEN_TYPE::EQUALS;));
    if (it == m_tokenList.end()) {
        return;
    }
    it = std::find_if(std::next(it), m_tokenList.end(), TT_LAMBDA(a, return a.m_type == Token::TOKEN_TYPE::EQUALS;));
    if (it != m_tokenList.end()) {
        m_success = false;
        m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::TOO_MANY_EQUALS, "", it->m_startIndexInString, it->m_endIndexInString});
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
        Token::TOKEN_TYPE::IDENTIFIER, Token::TOKEN_TYPE::NUM, Token::TOKEN_TYPE::LEFT_BR};

    for (auto it = m_tokenList.begin(); std::next(it) != m_tokenList.end(); ++it) {
        const auto currentType = it->m_type;
        if (operatorTypes.find(currentType) != operatorTypes.end()) {
            const auto nextType = std::next(it)->m_type;
            if (allowedAfterOperator.find(nextType) == allowedAfterOperator.end()) {
                m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::ILLEGAL_SEQUENCE,
                                                  it->m_string + " " + std::next(it)->m_string,
                                                  it->m_startIndexInString,
                                                  std::next(it)->m_endIndexInString});
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
                m_success = false;
                m_errors.emplace_back(ParserError{
                    ParserError::ERROR_TYPE::ILLEGAL_SEQUENCE, ", ,", std::prev(it)->m_startIndexInString, it->m_endIndexInString});
            } else {
                lastWasComma = true;
            }
        } else {
            lastWasComma = false;
        }
    }
}

void Tokenizer::initializeStructuralTokens() {
    for (const auto& token : m_tokenList) {
        m_structuralTokenList.emplace_back(token);
    }
}

std::string Tokenizer::structuralTokensToString() const {
    std::stringstream ss;
    ss << "Success: \t" << m_success << '\n';
    for (const auto& token : m_structuralTokenList) {
        ss << token.toString() << " ";
    }
    ss << '\n';
    return ss.str();
}

void Tokenizer::extractFunctionsAndBracketsFromStructuralTokens() {
    auto rightIt = std::find_if(TT_IT(m_structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR);));
    while (rightIt != m_structuralTokenList.end()) {
        auto leftIt = rightIt;
        while (not((--leftIt)->isRawTokenOfType(Token::TOKEN_TYPE::LEFT_BR))) {}

        assert(std::get<Token>(leftIt->m_token).m_additional == std::get<Token>(rightIt->m_token).m_additional);

        auto                       insertPosition = std::next(rightIt);
        std::list<StructuralToken> listInBrackets;
        if (leftIt != m_structuralTokenList.begin() && std::prev(leftIt)->isRawTokenOfType(Token::TOKEN_TYPE::IDENTIFIER)) {
            --leftIt;
        }
        listInBrackets.splice(listInBrackets.begin(), m_structuralTokenList, leftIt, std::next(rightIt));
        m_structuralTokenList.insert(insertPosition, listToStructuralToken(listInBrackets));

        rightIt = std::find_if(TT_IT(m_structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR);));
    }
}

std::string Tokenizer::toString(const std::list<StructuralToken>& structuralTokenList) {
    std::stringstream ss;
    for (const auto& token : structuralTokenList) {
        ss << token.toString();
    }
    ss << '\n';
    return ss.str();
}

StructuralToken Tokenizer::listToStructuralToken(std::list<StructuralToken> structuralTokenList) {
    bool isFunction = false;
    if (structuralTokenList.front().isRawTokenOfType(Token::TOKEN_TYPE::IDENTIFIER)) {
        isFunction = true;
    }
    auto commaIt = std::find_if(TT_IT(structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::COMMA);));
    if (commaIt == structuralTokenList.end()) {
        return isFunction ? StructuralToken{StructuralToken::Function{"", StructuralToken::Bracketed{structuralTokenList}}}
                          : StructuralToken{StructuralToken::Bracketed{structuralTokenList}};
    }
    std::vector<std::list<StructuralToken>> commaSeparatedGroups;
    while (commaIt != structuralTokenList.end()) {
        commaSeparatedGroups.emplace_back();
        commaSeparatedGroups.back().splice(commaSeparatedGroups.back().begin(), structuralTokenList, structuralTokenList.begin(), commaIt);
        structuralTokenList.pop_front();
        commaIt = std::find_if(TT_IT(structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::COMMA);));
    }
    std::list<StructuralToken> tokenList;
    tokenList.splice(tokenList.begin(), structuralTokenList, structuralTokenList.begin(), structuralTokenList.end());
    commaSeparatedGroups.emplace_back(std::move(tokenList));
    return isFunction ? StructuralToken{StructuralToken::Function{"", StructuralToken::MultiBracketed{std::move(commaSeparatedGroups)}}}
                      : StructuralToken{StructuralToken::MultiBracketed{std::move(commaSeparatedGroups)}};
}
