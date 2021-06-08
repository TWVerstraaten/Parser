//
// Created by pc on 07-06-21.
//

#include "StructuralToken.h"

#include "../gen/defines.h"

#include <algorithm>
#include <cassert>
#include <sstream>

StructuralToken::StructuralToken(const Token& token) : m_token(token), m_range(token.m_range) {
}

StructuralToken::StructuralToken(Bracketed&& multiBracketed, Range range) : m_token(multiBracketed), m_range(range) {
}

StructuralToken::StructuralToken(Function&& function, Range range) : m_token(function), m_range(range) {
}

StructuralToken::StructuralToken(const std::string& variable, Range range) : m_token(variable), m_range(range) {
}

StructuralToken::StructuralToken(double number, Range range) : m_token(number), m_range(range) {
}

StructuralToken::StructuralToken(long long number, Range range) : m_token(number), m_range(range) {
}

std::string StructuralToken::toString() const {
    return std::visit([this](const auto& a) { return toString(a); }, m_token);
}

std::string StructuralToken::toString(const Token& token) const {
    return "t" + m_range.toString() + "(" + token.toString(true) + ")";
}

std::string StructuralToken::toString(const std::string& token) const {
    return "x" + m_range.toString() + "(" + token + ")";
}

std::string StructuralToken::toString(double token) const {
    return "d" + m_range.toString() + "(" + std::to_string(token) + ")";
}

std::string StructuralToken::toString(long long token) const {
    return "n" + m_range.toString() + "(" + std::to_string(token) + ")";
}

std::string StructuralToken::toString(const Bracketed& token) const {
    std::stringstream ss;
    ss << "Br" << m_range.toString() << "(";
    bool writeComma = false;
    for (const auto& list : token.m_tokenLists) {
        if (writeComma) {
            ss << ", ";
        }
        writeComma = true;
        std::for_each(TT_IT(list), [&](const auto& a) { ss << a.toString(); });
    }
    ss << ")";
    return ss.str();
}

std::string StructuralToken::toString(const Function& token) const {
    std::stringstream ss;
    ss << "F" << m_range.toString() << token.m_name << "(";
    ss << toString(token.m_arguments);
    ss << ")";
    return ss.str();
}

bool StructuralToken::isRawTokenOfType(Token::TOKEN_TYPE type) const {
    return std::holds_alternative<Token>(m_token) && std::get<Token>(m_token).m_type == type;
}

bool StructuralToken::isString() const {
    return std::holds_alternative<std::string>(m_token);
}

StructuralToken StructuralToken::makeFromCommaSeparated(std::list<StructuralToken>&& tokenList) {
    assert(tokenList.size() > 1);
    assert(tokenList.back().isRawTokenOfType(Token::TOKEN_TYPE::RIGHT_BR));

    const size_t startIndex   = tokenList.front().m_range.m_startIndex;
    const size_t endIndex     = tokenList.back().m_range.m_endIndex;
    bool         isFunction   = tokenList.front().isString();
    std::string  functionName = isFunction ? std::get<std::string>(tokenList.front().m_token) : "";
    assert(isFunction != (functionName.empty()));
    assert(isFunction ? std::next(tokenList.begin())->isRawTokenOfType(Token::TOKEN_TYPE::LEFT_BR)
                      : tokenList.front().isRawTokenOfType(Token::TOKEN_TYPE::LEFT_BR));

    if (isFunction) {
        tokenList.pop_front();
    }
    tokenList.pop_front();
    tokenList.pop_back();
    return isFunction ? StructuralToken{StructuralToken::Function{functionName, makeBracketed(tokenList)}, {startIndex, endIndex}}
                      : StructuralToken{makeBracketed(tokenList), {startIndex, endIndex}};
}

StructuralToken::Bracketed StructuralToken::makeBracketed(std::list<StructuralToken>& tokenList) {
    const size_t commaCount = std::count_if(TT_IT(tokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::COMMA);));
    std::vector<std::list<StructuralToken>> commaSeparatedGroups(commaCount + 1);

    for (size_t i = 0; i != commaCount + 1; ++i) {
        const auto commaIt = std::find_if(TT_IT(tokenList), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::COMMA);));
        commaSeparatedGroups[i].splice(commaSeparatedGroups[i].begin(), tokenList, tokenList.begin(), commaIt);
        if (not tokenList.empty()) {
            tokenList.pop_front();
        }
    }
    return StructuralToken::Bracketed{std::move(commaSeparatedGroups)};
}
