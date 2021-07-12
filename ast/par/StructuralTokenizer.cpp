//
// Created by pc on 07-06-21.
//

#include "StructuralTokenizer.h"

#include "../../gen/Overloaded.h"
#include "../../gen/defines.h"
#include "../err/ParserInfo.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

namespace ast::par {

    StructuralTokenizer::StructuralTokenizer(const std::list<Token>& rawTokenList, err::ParserInfo& info) : m_info(info) {
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
            //            insertMultiplicationWhereNeeded(m_tokenList);
        }
    }

    void StructuralTokenizer::addStringTokenToStructuralTokens(const Token& token) {
        const auto parsedIdentifier = S_PARSE_IDENTIFIER_TOKEN(token);
        if (parsedIdentifier.has_value()) {
            m_tokenList.emplace_back(StructuralToken{parsedIdentifier.value(), token.range()});
        } else {
            m_info.add({err::ParserError::TYPE::IDENTIFIER_ERROR, token.string(), token.range()});
        }
    }

    void StructuralTokenizer::extractFunctionsAndBracketsFromStructuralTokens() {
        const size_t bracketCount = std::count_if(TT_IT(m_tokenList), TT_LAMBDA(a, return TokenTemplates::S_IS_TOKEN_OF_TYPE<Token>(a.token(), Token::TYPE::RIGHT_BR);));
        for (size_t dummy = 0; dummy != bracketCount; ++dummy) {
            auto rightIt = std::find_if(TT_IT(m_tokenList), TT_LAMBDA(a, return TokenTemplates::S_IS_TOKEN_OF_TYPE<Token>(a.token(), Token::TYPE::RIGHT_BR);));
            auto leftIt  = rightIt;
            while (not(TokenTemplates::S_IS_TOKEN_OF_TYPE<Token>((--leftIt)->token(), Token::TYPE::LEFT_BR))) {}

            if (leftIt != m_tokenList.begin() && std::prev(leftIt)->holdsString()) {
                --leftIt;
            }
            auto                       insertPosition = std::next(rightIt);
            std::list<StructuralToken> listInBrackets;
            listInBrackets.splice(listInBrackets.begin(), m_tokenList, leftIt, std::next(rightIt));
            m_tokenList.insert(insertPosition, StructuralToken::S_MAKE_FROM_COMMA_SEPARATED(std::move(listInBrackets)));
        }
    }

    void StructuralTokenizer::addNumberTokenToStructuralTokens(const Token& token) {
        const auto parsedNumber = S_PARSE_NUMBER_TOKEN(token);
        if (parsedNumber.has_value()) {
            std::visit([&](auto a) { m_tokenList.emplace_back(StructuralToken{a, token.range()}); }, parsedNumber.value());
        } else {
            m_info.add({err::ParserError::TYPE::NUMBER_ERROR, token.string(), token.range()});
        }
    }

    std::optional<std::string> StructuralTokenizer::S_PARSE_IDENTIFIER_TOKEN(const Token& token) {
        assert(token.type() == Token::TYPE::IDENTIFIER);
        const auto& string = token.string();
        if (string.empty() || not isalpha(string.front()) || std::find_if(TT_IT(string), TT_LAMBDA(a, return not(isalnum(a));)) != string.end()) {
            return {};
        }
        return string;
    }

    std::optional<std::variant<double, long long>> StructuralTokenizer::S_PARSE_NUMBER_TOKEN(const Token& token) {
        assert(token.type() == Token::TYPE::NUMBER);
        const auto&  string   = token.string();
        const size_t dotCount = std::count_if(TT_IT(string), TT_LAMBDA(a, return a == '.';));

        if (string.empty() || std::find_if(TT_IT(string), TT_LAMBDA(a, return not(isdigit(a) || a == '.');)) != string.end() || dotCount > 1) {
            return {};
        }
        try {
            if (dotCount == 0) {
                return std::variant<double, long long>(std::stoll(string));
            } else {
                return std::variant<double, long long>(std::stod(string.at(0) == '.' ? "0" + string : string));
            }
        } catch (...) { return {}; }
    }

    std::string StructuralTokenizer::S_TO_STRING(const std::list<StructuralToken>& structuralTokenList) {
        std::stringstream ss;
        for (const auto& token : structuralTokenList) {
            ss << token.toString();
        }
        ss << '\n';
        return ss.str();
    }

    std::string StructuralTokenizer::toString() const {
        return S_TO_STRING(m_tokenList);
    }

    const std::list<StructuralToken>& StructuralTokenizer::tokenList() const {
        return m_tokenList;
    }

} // namespace ast::par