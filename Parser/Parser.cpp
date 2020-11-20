//
// Created by pc on 19-11-20.
//

#include "Parser.h"

#include "AstNode/AstNodeAdd.h"
#include "AstNode/AstNodeDiv.h"
#include "AstNode/AstNodeDouble.h"
#include "AstNode/AstNodeError.h"
#include "AstNode/AstNodeFunction.h"
#include "AstNode/AstNodeInteger.h"
#include "AstNode/AstNodeMul.h"
#include "AstNode/AstNodePower.h"
#include "AstNode/AstNodeSubtract.h"
#include "AstNode/AstNodeUnaryMinus.h"
#include "AstNode/AstNodeVar.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

static Tokenizer::TokenList::const_iterator lastBeforeOccurrenceOfType(Tokenizer::TokenList::const_iterator       begin,
                                                                       const Tokenizer::TokenList::const_iterator end,
                                                                       const Tokenizer::TOKEN_TYPE& type) {
    while (true) {
        const auto next = std::next(begin);
        if (next->m_type == type) {
            return begin;
        }
        if (next == end) {
            return end;
        }
        ++begin;
    }
}

std::unique_ptr<AstNode> Parser::parse(std::string string) {
    return Parser::parse(Tokenizer(string).tokens());
}

std::unique_ptr<AstNode> Parser::parse(const Tokenizer::TokenList& tokenList) {
    std::cout << Tokenizer::toString(tokenList) << '\n';
    Parser parser;
    return parser.parseFunctions(tokenList);
}

std::unique_ptr<AstNode> Parser::parseNoFunctions(Tokenizer::TokenList tokenList) {
    return parseBrackets(tokenList);
}

std::unique_ptr<AstNode> Parser::parseValueType(const Tokenizer::Token& token) {
    switch (token.m_type) {
        case Tokenizer::TOKEN_TYPE::REFERENCE: {
            const auto indexString = token.m_string.substr(1);
            const auto index       = std::stoul(indexString);
            assert(index < m_subTokenLists.size());
            return std::unique_ptr<AstNode>(m_subTokenLists.at(index).release());
        }
        case Tokenizer::TOKEN_TYPE::IDENTIFIER:
            return std::unique_ptr<AstNode>(new AstNodeVar(token.m_string));
        case Tokenizer::TOKEN_TYPE::DOUBLE:
            return std::unique_ptr<AstNode>(new AstNodeDouble(token.m_string));
        case Tokenizer::TOKEN_TYPE::INTEGER:
            return std::unique_ptr<AstNode>(new AstNodeInteger(token.m_string));
        default:
            assert(false);
    }
}

bool isValueType(const Tokenizer::Token& token) {
    return token.m_type == Tokenizer::TOKEN_TYPE::INTEGER || token.m_type == Tokenizer::TOKEN_TYPE::DOUBLE ||
           token.m_type == Tokenizer::TOKEN_TYPE::IDENTIFIER || token.m_type == Tokenizer::TOKEN_TYPE::REFERENCE;
}

std::unique_ptr<AstNode> Parser::parseTerm(Tokenizer::TokenList& tokenList) {
    assert(std::find_if(tokenList.begin(), tokenList.end(), [](const Tokenizer::Token& token) {
               return token.m_type == Tokenizer::TOKEN_TYPE::BIN_OP_FAC ||
                      token.m_type == Tokenizer::TOKEN_TYPE::BIN_OP_EXPR;
           }) == tokenList.end());

    std::unique_ptr<AstNode> leadingTerm = parseLeadingValueType(tokenList);
    if (tokenList.empty()) {
        return leadingTerm;
    } else {
        const auto binOpIt = tokenList.begin();
        assert(binOpIt->m_type == Tokenizer::TOKEN_TYPE::BIN_OP_TERM);
        if (binOpIt->m_string == "+") {
            tokenList.pop_front();
            return std::unique_ptr<AstNode>(new AstNodeAdd(std::move(leadingTerm), parseTerm(tokenList)));
        } else if (binOpIt->m_string == "-") {
            tokenList.pop_front();
            return std::unique_ptr<AstNode>(new AstNodeSubtract(std::move(leadingTerm), parseTerm(tokenList)));
        } else {
            return std::unique_ptr<AstNode>(new AstNodeError{});
        }
    }
}

std::unique_ptr<AstNode> Parser::parseFactor(Tokenizer::TokenList& tokenList) {
    assert(not tokenList.empty());
    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), Tokenizer::TOKEN_TYPE::BIN_OP_FAC);
    if (it == tokenList.end()) {
        return parseTerm(tokenList);
    } else {
        it = tokenList.insert(it, freshReferenceToken());
        it = std::next(it);
        if (std::next(it)->m_string == "*") {
            m_subTokenLists.emplace_back(
                std::unique_ptr<AstNode>(new AstNodeMul(parseValueType(*it), parseValueType(*std::next(it, 2)))));
        } else {
            m_subTokenLists.emplace_back(
                std::unique_ptr<AstNode>(new AstNodeDiv(parseValueType(*it), parseValueType(*std::next(it, 2)))));
        }
        tokenList.erase(it, std::next(it, 3));
    }
    return parseFactor(tokenList);
}

std::unique_ptr<AstNode> Parser::parseExpression(Tokenizer::TokenList& tokenList) {
    assert(not tokenList.empty());
    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), Tokenizer::TOKEN_TYPE::BIN_OP_EXPR);
    if (it == tokenList.end()) {
        return parseFactor(tokenList);
    } else {
        it = tokenList.insert(it, freshReferenceToken());
        it = std::next(it);
        m_subTokenLists.emplace_back(
            std::unique_ptr<AstNode>(new AstNodePower(parseValueType(*it), parseValueType(*std::next(it, 2)))));
        tokenList.erase(it, std::next(it, 3));
    }
    return parseExpression(tokenList);
}

std::unique_ptr<AstNode> Parser::parseBrackets(Tokenizer::TokenList& tokenList) {
    auto openBracketIt = std::find_if(tokenList.begin(), tokenList.end(), [](const Tokenizer::Token& token) {
        return token.m_type == Tokenizer::TOKEN_TYPE::LEFT_BR;
    });
    while (openBracketIt != tokenList.end()) {
        auto closingBracketIt = std::find(openBracketIt, tokenList.end(),
                                          Tokenizer::Token{Tokenizer::TOKEN_TYPE::RIGHT_BR, openBracketIt->m_string});
        assert(closingBracketIt != tokenList.end());

        openBracketIt    = tokenList.insert(openBracketIt, freshReferenceToken());
        openBracketIt    = tokenList.erase(std::next(openBracketIt));
        closingBracketIt = tokenList.erase(closingBracketIt);

        Tokenizer::TokenList expressionInBrackets;
        expressionInBrackets.splice(expressionInBrackets.begin(), tokenList, openBracketIt, closingBracketIt);

        const size_t indexOfNewExpression = m_subTokenLists.size();
        m_subTokenLists.emplace_back(nullptr);
        m_subTokenLists[indexOfNewExpression] = parseNoFunctions(expressionInBrackets);

        openBracketIt = std::find_if(tokenList.begin(), tokenList.end(), [](const Tokenizer::Token& token) {
            return token.m_type == Tokenizer::TOKEN_TYPE::LEFT_BR;
        });
    }
    return parseExpression(tokenList);
}

std::string Parser::toString(const Tokenizer::TokenList& tokenList) const {
    std::string result;
    for (const auto& token : tokenList) {
        switch (token.m_type) {
            case Tokenizer::TOKEN_TYPE::REFERENCE: {
                const auto indexString = token.m_string.substr(1);
                const auto index       = std::stoul(indexString);
                assert(index < m_subTokenLists.size());
                result += m_subTokenLists.at(index)->toString();
                break;
            }
            case Tokenizer::TOKEN_TYPE::LEFT_BR:
                result += std::string("(");
                break;
            case Tokenizer::TOKEN_TYPE::RIGHT_BR:
                result += std::string(")");
                break;
            default:
                result += token.m_string;
                break;
        }
    }
    return result;
}

Tokenizer::Token Parser::freshReferenceToken() const {
    return Tokenizer::Token{Tokenizer::TOKEN_TYPE::REFERENCE, "$" + std::to_string(m_subTokenLists.size())};
}
std::unique_ptr<AstNode> Parser::parseLeadingValueType(Tokenizer::TokenList& tokenList) {
    std::unique_ptr<AstNode> leadingTerm;
    if (tokenList.begin()->m_type == Tokenizer::TOKEN_TYPE::UNARY_OP) {
        tokenList.pop_front();
        assert(isValueType(tokenList.front()));
        leadingTerm = std::make_unique<AstNodeUnaryMinus>(parseValueType(*tokenList.begin()));
        tokenList.pop_front();
    } else {
        assert(isValueType(tokenList.front()));
        leadingTerm = parseValueType(*tokenList.begin());
        tokenList.pop_front();
    }
    return leadingTerm;
}

std::unique_ptr<AstNode> Parser::parseFunctions(Tokenizer::TokenList tokenList) {
    auto functionIdIt = std::find_if(tokenList.begin(), tokenList.end(), [](const Tokenizer::Token& token) {
        return token.m_type == Tokenizer::TOKEN_TYPE::FUNCTION;
    });
    while (functionIdIt != tokenList.end()) {
        auto openBracketIt    = std::next(functionIdIt);
        auto closingBracketIt = std::find(functionIdIt, tokenList.end(),
                                          Tokenizer::Token{Tokenizer::TOKEN_TYPE::RIGHT_BR, openBracketIt->m_string});
        assert(closingBracketIt != tokenList.end());

        const std::string functionName = functionIdIt->m_string;

        functionIdIt     = tokenList.erase(functionIdIt);
        functionIdIt     = tokenList.erase(functionIdIt);
        closingBracketIt = tokenList.erase(closingBracketIt);
        functionIdIt     = tokenList.insert(functionIdIt, freshReferenceToken());

        Tokenizer::TokenList expressionInBrackets;
        expressionInBrackets.splice(expressionInBrackets.begin(), tokenList, std::next(functionIdIt), closingBracketIt);

        const size_t indexOfNewExpression = m_subTokenLists.size();
        m_subTokenLists.emplace_back(nullptr);
        m_subTokenLists[indexOfNewExpression].reset(new AstNodeFunction(functionName, parse(expressionInBrackets)));

        functionIdIt = std::find_if(tokenList.begin(), tokenList.end(), [](const Tokenizer::Token& token) {
            return token.m_type == Tokenizer::TOKEN_TYPE::FUNCTION;
        });
    }
    return parseNoFunctions(tokenList);
}
