//
// Created by pc on 07-06-21.
//

#include "StructuralToken.h"

#include "../gen/defines.h"
#include "TokenWriter.h"

#include <algorithm>
#include <cassert>

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
    return std::visit([this](const auto& a) { return TokenWriter::toString(a, m_range); }, m_token);
}

bool StructuralToken::isString() const {
    return std::holds_alternative<std::string>(m_token);
}

StructuralToken StructuralToken::makeFromCommaSeparated(std::list<StructuralToken>&& tokenList) {
    assert(tokenList.size() > 1);
    assert(Token::isTokenOfType(tokenList.back().m_token, Token::TYPE::RIGHT_BR));

    const size_t startIndex   = tokenList.front().m_range.startIndex();
    const size_t endIndex     = tokenList.back().m_range.endIndex();
    bool         isFunction   = tokenList.front().isString();
    std::string  functionName = isFunction ? std::get<std::string>(tokenList.front().m_token) : "";
    assert(isFunction != (functionName.empty()));
    assert(isFunction ? Token::isTokenOfType(std::next(tokenList.begin())->m_token, Token::TYPE::LEFT_BR)
                      : Token::isTokenOfType(tokenList.front().m_token, Token::TYPE::LEFT_BR));

    if (isFunction) {
        tokenList.pop_front();
    }
    tokenList.pop_front();
    tokenList.pop_back();
    return isFunction ? StructuralToken{StructuralToken::Function{functionName, makeBracketed(tokenList)}, {startIndex, endIndex}}
                      : StructuralToken{makeBracketed(tokenList), {startIndex, endIndex}};
}

StructuralToken::Bracketed StructuralToken::makeBracketed(std::list<StructuralToken>& tokenList) {
    const size_t commaCount = std::count_if(TT_IT(tokenList), TT_LAMBDA(a, return Token::isTokenOfType(a.m_token, Token::TYPE::COMMA);));
    std::vector<std::list<StructuralToken>> commaSeparatedGroups(commaCount + 1);

    for (size_t i = 0; i != commaCount + 1; ++i) {
        const auto commaIt = std::find_if(TT_IT(tokenList), TT_LAMBDA(a, return Token::isTokenOfType(a.m_token, Token::TYPE::COMMA);));
        commaSeparatedGroups[i].splice(commaSeparatedGroups[i].begin(), tokenList, tokenList.begin(), commaIt);
        if (not tokenList.empty()) {
            tokenList.pop_front();
        }
    }
    return StructuralToken::Bracketed{std::move(commaSeparatedGroups)};
}
