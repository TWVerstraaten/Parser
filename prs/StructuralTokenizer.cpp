//
// Created by pc on 07-06-21.
//

#include "StructuralTokenizer.h"

#include "../gen/defines.h"
#include "Token.h"

#include <algorithm>
#include <cassert>
#include <sstream>

StructuralTokenizer::StructuralTokenizer(const std::list<Token>& m_rawTokenList, ParserInfo& info) : m_info(info) {
    for (const auto& token : m_rawTokenList) {
        switch (token.m_type) {
            case Token::TOKEN_TYPE::NUMBER:
                addNumberTokenToStructuralTokens(token);
                break;
            case Token::TOKEN_TYPE::IDENTIFIER:
                addStringTokenToStructuralTokens(token);
                break;
            default:
                m_tokenList.emplace_back(token);
                break;
        }
    }
    extractFunctionsAndBracketsFromStructuralTokens();
}

void StructuralTokenizer::addStringTokenToStructuralTokens(const Token& token) {
    const auto parsedIdentifier = parseIdentifierToken(token);
    if (parsedIdentifier.has_value()) {
        m_tokenList.emplace_back(StructuralToken{parsedIdentifier.value(), token.m_startIndex, token.m_endIndex});
    } else {
        m_info.addError(ParserError{ParserError::ERROR_TYPE::IDENTIFIER_ERROR, token.m_string, token.m_startIndex, token.m_endIndex});
    }
}

void StructuralTokenizer::extractFunctionsAndBracketsFromStructuralTokens() {
    const size_t bracketCount = std::count_if(TT_IT(m_tokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR);));
    for (size_t dummy = 0; dummy != bracketCount; ++dummy) {
        auto rightIt = std::find_if(TT_IT(m_tokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR);));
        auto leftIt  = rightIt;
        while (not((--leftIt)->isRawTokenOfType(Token::TOKEN_TYPE::LEFT_BR))) {}
        assert(std::get<Token>(leftIt->m_token).m_additional == std::get<Token>(rightIt->m_token).m_additional);

        if (leftIt != m_tokenList.begin() && std::prev(leftIt)->isString()) {
            --leftIt;
        }
        auto                       insertPosition = std::next(rightIt);
        std::list<StructuralToken> listInBrackets;
        listInBrackets.splice(listInBrackets.begin(), m_tokenList, leftIt, std::next(rightIt));
        m_tokenList.insert(insertPosition, StructuralToken::makeFromCommaSeparated(std::move(listInBrackets)));
    }
}

void StructuralTokenizer::addNumberTokenToStructuralTokens(const Token& token) {
    const auto parsedNumber = parseNumberToken(token);
    if (parsedNumber.has_value()) {
        std::visit(
            [&](auto a) {
                m_tokenList.emplace_back(StructuralToken{a, token.m_startIndex, token.m_endIndex});
            },
            parsedNumber.value());
    } else {
        m_info.addError(ParserError{ParserError::ERROR_TYPE::NUMBER_ERROR, token.m_string, token.m_startIndex, token.m_endIndex});
    }
}

std::optional<std::string> StructuralTokenizer::parseIdentifierToken(const Token& token) {
    assert(token.m_type == Token::TOKEN_TYPE::IDENTIFIER);
    const auto& string = token.m_string;
    if (string.empty() || not isalpha(string.front()) ||
        std::find_if(TT_IT(string), TT_LAMBDA(a, return not(isalnum(a));)) != string.end()) {
        return {};
    }
    return string;
}

std::optional<std::variant<double, long long>> StructuralTokenizer::parseNumberToken(const Token& token) {
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

std::string StructuralTokenizer::toString(const std::list<StructuralToken>& structuralTokenList) {
    std::stringstream ss;
    for (const auto& token : structuralTokenList) {
        ss << token.toString();
    }
    ss << '\n';
    return ss.str();
}

std::string StructuralTokenizer::toString() const {
    return toString(m_tokenList);
}
