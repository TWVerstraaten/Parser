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
                    m_tokenList.emplace_back(Token{Token::TOKEN_TYPE::NUMBER, m_string.substr(i, j - i), i, j});
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
                m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::UNMATCHED_CLOSING_BR, "", token.m_startIndex, token.m_endIndex});
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
        m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::UNMATCHED_OPEN_BR, "", it->m_startIndex, it->m_endIndex});
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
        m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::TOO_MANY_EQUALS, "", it->m_startIndex, it->m_endIndex});
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
                m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::ILLEGAL_SEQUENCE,
                                                  it->m_string + " " + std::next(it)->m_string,
                                                  it->m_startIndex,
                                                  std::next(it)->m_endIndex});
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
                m_errors.emplace_back(
                    ParserError{ParserError::ERROR_TYPE::ILLEGAL_SEQUENCE, ", ,", std::prev(it)->m_startIndex, it->m_endIndex});
            } else {
                lastWasComma = true;
            }
        } else {
            lastWasComma = false;
        }
    }
}

std::optional<std::string> Tokenizer::parseIdentifierToken(const Token& token) {
    assert(token.m_type == Token::TOKEN_TYPE::IDENTIFIER);
    const auto& string = token.m_string;
    if (string.empty() || not isalpha(string.front()) ||
        std::find_if(TT_IT(string), TT_LAMBDA(a, return not(isalnum(a));)) != string.end()) {
        return {};
    }
    return string;
}

std::optional<std::variant<double, long long>> Tokenizer::parseNumberToken(const Token& token) {
    assert(token.m_type == Token::TOKEN_TYPE::NUMBER);
    const auto&  string   = token.m_string;
    const size_t dotCount = std::count_if(TT_IT(string), TT_LAMBDA(a, return a == '.';));

    if (string.empty() || std::find_if(TT_IT(string), TT_LAMBDA(a, return not(isdigit(a) || a == '.');)) != string.end() || dotCount > 1) {
        return {};
    }
    try {
        return dotCount == 0 ? std::variant<double, long long>(std::stoll(string)) : std::stod(string);
    } catch (...) { return {}; }
}

void Tokenizer::addStringTokenToStructuralTokens(const Token& token) {
    const auto parsedIdentifier = parseIdentifierToken(token);
    if (parsedIdentifier.has_value()) {
        m_structuralTokenList.emplace_back(StructuralToken{parsedIdentifier.value(), token.m_startIndex, token.m_endIndex});
    } else {
        m_success = false;
        m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::IDENTIFIER_ERROR, token.m_string, token.m_startIndex, token.m_endIndex});
    }
}

void Tokenizer::addNumberTokenToStructuralTokens(const Token& token) {
    const auto parsedNumber = parseNumberToken(token);
    if (parsedNumber.has_value()) {
        std::visit(
            [&](auto a) {
                m_structuralTokenList.emplace_back(StructuralToken{a, token.m_startIndex, token.m_endIndex});
            },
            parsedNumber.value());
    } else {
        m_success = false;
        m_errors.emplace_back(ParserError{ParserError::ERROR_TYPE::NUMBER_ERROR, token.m_string, token.m_startIndex, token.m_endIndex});
    }
}

void Tokenizer::initializeStructuralTokens() {
    for (const auto& token : m_tokenList) {
        switch (token.m_type) {
            case Token::TOKEN_TYPE::NUMBER:
                addNumberTokenToStructuralTokens(token);
                break;
            case Token::TOKEN_TYPE::IDENTIFIER:
                addStringTokenToStructuralTokens(token);
                break;
            default:
                m_structuralTokenList.emplace_back(token);
                break;
        }
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
    const size_t bracketCount =
        std::count_if(TT_IT(m_structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR);));
    for (size_t dummy = 0; dummy != bracketCount; ++dummy) {
        auto rightIt = std::find_if(TT_IT(m_structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR);));
        auto leftIt  = rightIt;
        while (not((--leftIt)->isRawTokenOfType(Token::TOKEN_TYPE::LEFT_BR))) {}
        assert(std::get<Token>(leftIt->m_token).m_additional == std::get<Token>(rightIt->m_token).m_additional);

        if (leftIt != m_structuralTokenList.begin() && std::prev(leftIt)->isString()) {
            --leftIt;
        }
        auto                       insertPosition = std::next(rightIt);
        std::list<StructuralToken> listInBrackets;
        listInBrackets.splice(listInBrackets.begin(), m_structuralTokenList, leftIt, std::next(rightIt));
        m_structuralTokenList.insert(insertPosition, StructuralToken::makeFromCommaSeparated(std::move(listInBrackets)));
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
//
// StructuralToken Tokenizer::listToStructuralToken(std::list<StructuralToken> structuralTokenList) {
//    bool         isFunction = structuralTokenList.front().isRawTokenOfType(Token::TOKEN_TYPE::IDENTIFIER);
//    const size_t commaCount = std::count_if(TT_IT(structuralTokenList), TT_LAMBDA(a, return
//    a.isRawTokenOfType(Token::TOKEN_TYPE::COMMA);)); std::vector<std::list<StructuralToken>> commaSeparatedGroups(commaCount + 1); for
//    (size_t i = 0; i != commaCount + 1; ++i) {
//        const auto commaIt = std::find_if(TT_IT(structuralTokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::COMMA);));
//        commaSeparatedGroups[i].splice(commaSeparatedGroups[i].begin(), structuralTokenList, structuralTokenList.begin(), commaIt);
//        if (not structuralTokenList.empty()) {
//            structuralTokenList.pop_front();
//        }
//    }
//    return isFunction ? StructuralToken{StructuralToken::Function{"", StructuralToken::MultiBracketed{std::move(commaSeparatedGroups)}}}
//                      : StructuralToken{StructuralToken::MultiBracketed{std::move(commaSeparatedGroups)}};
//}
