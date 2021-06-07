//
// Created by pc on 07-06-21.
//

#include "StructuralToken.h"

#include "../gen/defines.h"

#include <algorithm>
#include <cassert>
#include <sstream>

StructuralToken::StructuralToken(const Token& token) : m_token(token), m_startIndex(token.m_startIndex), m_endIndex(token.m_endIndex) {
}

StructuralToken::StructuralToken(Bracketed&& multiBracketed, size_t startIndex, size_t endIndex)
    : m_token(multiBracketed), m_startIndex(startIndex), m_endIndex(endIndex) {
}

StructuralToken::StructuralToken(Function&& function, size_t startIndex, size_t endIndex)
    : m_token(function), m_startIndex(startIndex), m_endIndex(endIndex) {
}

StructuralToken::StructuralToken(const std::string& variable, size_t startIndex, size_t endIndex)
    : m_token(variable), m_startIndex(startIndex), m_endIndex(endIndex) {
}

StructuralToken::StructuralToken(double number, size_t startIndex, size_t endIndex)
    : m_token(number), m_startIndex(startIndex), m_endIndex(endIndex) {
}

StructuralToken::StructuralToken(long long number, size_t startIndex, size_t endIndex)
    : m_token(number), m_startIndex(startIndex), m_endIndex(endIndex) {
}

std::string StructuralToken::toString() const {
    return std::visit([this](const auto& a) { return toString(a); }, m_token);
}

std::string StructuralToken::toString(const Token& token) const {
    return "t[" + std::to_string(m_startIndex) + "," + std::to_string(m_endIndex) + "](" + token.toString(true) + ")";
}

std::string StructuralToken::toString(const std::string& token) const {
    return "x[" + std::to_string(m_startIndex) + "," + std::to_string(m_endIndex) + "](" + token + ")";
}

std::string StructuralToken::toString(double token) const {
    return "d[" + std::to_string(m_startIndex) + "," + std::to_string(m_endIndex) + "](" + std::to_string(token) + ")";
}

std::string StructuralToken::toString(long long token) const {
    return "n[" + std::to_string(m_startIndex) + "," + std::to_string(m_endIndex) + "](" + std::to_string(token) + ")";
}

std::string StructuralToken::toString(const Bracketed& token) const {
    std::stringstream ss;
    ss << "Br[" << std::to_string(m_startIndex) << "," << std::to_string(m_endIndex) << "](";
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
    ss << "F[" + std::to_string(m_startIndex) << "," << std::to_string(m_endIndex) << "]" << token.m_name << "(";
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

    const size_t startIndex   = tokenList.front().m_startIndex;
    const size_t endIndex     = tokenList.back().m_endIndex;
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
    if (isFunction) {
        return StructuralToken{StructuralToken::Function{functionName, makeBracketed(tokenList)}, startIndex, endIndex};
    } else {
        return StructuralToken{makeBracketed(tokenList), startIndex, endIndex};
    }
}

StructuralToken::Bracketed StructuralToken::makeBracketed(std::list<StructuralToken>& tokenList) {
    if (tokenList.empty()) {
        return Bracketed{{}};
    }

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
