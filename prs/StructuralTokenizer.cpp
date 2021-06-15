//
// Created by pc on 07-06-21.
//

#include "StructuralTokenizer.h"

#include "../gen/Overloaded.h"
#include "../gen/defines.h"
#include "ParserInfo.h"

#include <algorithm>
#include <cassert>
#include <sstream>

StructuralTokenizer::StructuralTokenizer(const std::list<Token>& rawTokenList, ParserInfo& info) : m_info(info) {
    if (not m_info.success()) {
        return;
    }

    for (const auto& token : rawTokenList) {
        switch (token.type()) {
            case Token::TYPE::NUMBER:
                addNumberTokenToStructuralTokens(token);
                break;
            case Token::TYPE::IDENTIFIER:
                addStringTokenToStructuralTokens(token);
                break;
            default:
                m_tokenList.emplace_back(token);
                break;
        }
    }
    if (info.success()) {
        extractFunctionsAndBracketsFromStructuralTokens();
        insertMultiplicationWhereNeeded(m_tokenList);
    }
}

void StructuralTokenizer::addStringTokenToStructuralTokens(const Token& token) {
    const auto parsedIdentifier = parseIdentifierToken(token);
    if (parsedIdentifier.has_value()) {
        m_tokenList.emplace_back(StructuralToken{parsedIdentifier.value(), token.range()});
    } else {
        m_info.addError(ParserError{ParserError::TYPE::IDENTIFIER_ERROR, token.string(), token.range()});
    }
}

void StructuralTokenizer::extractFunctionsAndBracketsFromStructuralTokens() {
    const size_t bracketCount = std::count_if(TT_IT(m_tokenList), TT_LAMBDA(a, return TokenTemplates::isTokenOfType<Token>(a.token(), Token::TYPE::RIGHT_BR);));
    for (size_t dummy = 0; dummy != bracketCount; ++dummy) {
        auto rightIt = std::find_if(TT_IT(m_tokenList), TT_LAMBDA(a, return TokenTemplates::isTokenOfType<Token>(a.token(), Token::TYPE::RIGHT_BR);));
        auto leftIt  = rightIt;
        while (not(TokenTemplates::isTokenOfType<Token>((--leftIt)->token(), Token::TYPE::LEFT_BR))) {}

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
        std::visit([&](auto a) { m_tokenList.emplace_back(StructuralToken{a, token.range()}); }, parsedNumber.value());
    } else {
        m_info.addError(ParserError{ParserError::TYPE::NUMBER_ERROR, token.string(), token.range()});
    }
}

std::optional<std::string> StructuralTokenizer::parseIdentifierToken(const Token& token) {
    assert(token.type() == Token::TYPE::IDENTIFIER);
    const auto& string = token.string();
    if (string.empty() || not isalpha(string.front()) || std::find_if(TT_IT(string), TT_LAMBDA(a, return not(isalnum(a));)) != string.end()) {
        return {};
    }
    return string;
}

std::optional<std::variant<double, long long>> StructuralTokenizer::parseNumberToken(const Token& token) {
    assert(token.type() == Token::TYPE::NUMBER);
    const auto&  string   = token.string();
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

const std::list<StructuralToken>& StructuralTokenizer::tokenList() const {
    return m_tokenList;
}

void StructuralTokenizer::insertMultiplicationWhereNeeded(std::list<StructuralToken>& structuralTokens) {
    if (structuralTokens.empty()) {
        return;
    }
    for (auto it = structuralTokens.begin(); it != structuralTokens.end(); ++it) {
        if (std::next(it) != structuralTokens.end() && not(std::holds_alternative<Token>((*it).token()) || std::holds_alternative<Token>((*std::next(it)).token()))) {
            m_info.addMessage({ParserMessage::TYPE::INSERT_MULTIPLICATION, "", {it->range().endIndex() + 1, it->range().endIndex() + 1}});
            it = structuralTokens.insert(std::next(it), StructuralToken{Token{Token::TYPE::TIMES, "* (inferred)", {}}});
        }
        std::visit(
            Overloaded{
                [this](StructuralToken::Function& function) { std::for_each(TT_IT(function.m_arguments.m_tokenLists), [this](auto& a) { insertMultiplicationWhereNeeded(a); }); },
                [this](StructuralToken::Bracketed& bracketed) { std::for_each(TT_IT(bracketed.m_tokenLists), [this](auto& a) { insertMultiplicationWhereNeeded(a); }); },
                [](const auto) {}},
            (*it).token());
    }
}
