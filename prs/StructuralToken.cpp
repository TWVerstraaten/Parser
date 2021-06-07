//
// Created by pc on 07-06-21.
//

#include "StructuralToken.h"

#include <cassert>

StructuralToken::StructuralToken(const Token& token)
    : m_token(token), m_startIndexInString(token.m_startIndexInString), m_endIndexInString(token.m_endIndexInString) {
}

StructuralToken::StructuralToken(const MultiBracketed& multiBracketed) : m_token(multiBracketed) {
    m_startIndexInString = multiBracketed.m_tokenLists.front().front().m_startIndexInString;
    m_endIndexInString   = multiBracketed.m_tokenLists.back().back().m_endIndexInString;
    std::get<MultiBracketed>(m_token).m_tokenLists.front().pop_front();
    std::get<MultiBracketed>(m_token).m_tokenLists.back().pop_back();
}

StructuralToken::StructuralToken(const Function& function) : m_token(function) {
    auto& fun = std::get<Function>(m_token);
    if (std::holds_alternative<Bracketed>(fun.m_arguments)) {
        fun.m_name = std::get<Token>(std::get<Bracketed>(fun.m_arguments).m_tokenList.front().m_token).m_string;
        std::get<Bracketed>(fun.m_arguments).m_tokenList.pop_front();
        std::get<Bracketed>(fun.m_arguments).m_tokenList.pop_front();
    } else if (std::holds_alternative<MultiBracketed>(fun.m_arguments)) {
        fun.m_name = std::get<Token>(std::get<MultiBracketed>(fun.m_arguments).m_tokenLists.front().front().m_token).m_string;
        std::get<MultiBracketed>(fun.m_arguments).m_tokenLists.front().pop_front();
        std::get<MultiBracketed>(fun.m_arguments).m_tokenLists.front().pop_front();
    } else {
        assert(false);
    }
}

StructuralToken::StructuralToken(const Bracketed& bracketed) : m_token(bracketed) {
    m_startIndexInString = bracketed.m_tokenList.front().m_startIndexInString;
    m_endIndexInString   = bracketed.m_tokenList.back().m_endIndexInString;
    std::get<Bracketed>(m_token).m_tokenList.pop_front();
    std::get<Bracketed>(m_token).m_tokenList.pop_back();
}

std::string StructuralToken::toString() const {
    return std::visit([this](const auto& a) { return toString(a); }, m_token);
}

std::string StructuralToken::toString(const Token& token) const {
    return token.toString(true);
}

std::string StructuralToken::toString(const Bracketed& token) const {
    std::string result = "Br(";
    for (const auto& el : token.m_tokenList) {
        result += el.toString();
    }
    result += ")";
    return result;
}

std::string StructuralToken::toString(const MultiBracketed& token) const {
    std::string result = "MultiBr(";
    for (const auto& list : token.m_tokenLists) {
        for (const auto& el : list) {
            result += el.toString();
        }
        result += +", ";
    }
    result = result.substr(0, result.length() - 2);
    result += ")";
    return result;
}

std::string StructuralToken::toString(const Function& token) const {
    std::string result = "Fun_@" + token.m_name + "(";
    result += std::visit([this](const auto& a) { return toString(a); }, token.m_arguments);
    result += ")";
    return result;
}

bool StructuralToken::isRawTokenOfType(Token::TOKEN_TYPE type) const {
    return std::holds_alternative<Token>(m_token) && std::get<Token>(m_token).m_type == type;
}
