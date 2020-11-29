//
// Created by pc on 19-11-20.
//

#include "Parser.h"

#include "AstNode/AstNodeAdd.h"
#include "AstNode/AstNodeDiv.h"
#include "AstNode/AstNodeDouble.h"
#include "AstNode/AstNodeFunction.h"
#include "AstNode/AstNodeInteger.h"
#include "AstNode/AstNodeMul.h"
#include "AstNode/AstNodePower.h"
#include "AstNode/AstNodeUnaryMinus.h"
#include "AstNode/AstNodeVar.h"

#include <cassert>
#include <functional>
#include <memory>
#include <utility>

using TokenList  = Tokenizer::TokenList;
using Token      = Tokenizer::Token;
using TOKEN_TYPE = Tokenizer::TOKEN_TYPE;

static TokenList::const_iterator lastBeforeOccurrenceOfType(TokenList::const_iterator       begin,
                                                            const TokenList::const_iterator end,
                                                            const TOKEN_TYPE&               type) {
    while (std::next(begin) != end) {
        const auto next = std::next(begin);
        if (next->m_type == type) {
            return begin;
        }
        begin = std::next(begin);
    }
    return end;
}

bool containsFactor(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(),
                        [](const Token& token) { return token.m_type == TOKEN_TYPE::BIN_OP_FAC; }) != tokenList.end();
}

bool containsExpression(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(),
                        [](const Token& token) { return token.m_type == TOKEN_TYPE::BIN_OP_EXPR; }) != tokenList.end();
}

bool containsFunction(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(),
                        [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; }) != tokenList.end();
}

bool containsBrackets(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(),
                        [](const Token& token) { return token.m_type == TOKEN_TYPE::LEFT_BR; }) != tokenList.end();
}

u_ptr_AstNode Parser::parse(const std::string& string) {
    return Parser::parse(Tokenizer(string).tokenList());
}

u_ptr_AstNode Parser::parse(const TokenList& tokenList) {
    Parser parser;
    return parser.parseFunctions(tokenList);
}

u_ptr_AstNode Parser::parseValueType(const Token& token) {
    switch (token.m_type) {
        case TOKEN_TYPE::REFERENCE: {
            const auto indexString = token.m_string.substr(1);
            const auto index       = std::stoul(indexString);
            assert(index < m_subExpresssionList.size());
            return std::move(m_subExpresssionList.at(index));
        }
        case TOKEN_TYPE::IDENTIFIER:
            return u_ptr_AstNode(new AstNodeVar(token.m_string));
        case TOKEN_TYPE::DOUBLE:
            return u_ptr_AstNode(new AstNodeDouble(token.m_string));
        case TOKEN_TYPE::INTEGER:
            return u_ptr_AstNode(new AstNodeInteger(token.m_string));
        default:
            assert(false);
    }
}

bool isValueType(const Token& token) {
    return token.m_type == TOKEN_TYPE::INTEGER || token.m_type == TOKEN_TYPE::DOUBLE ||
           token.m_type == TOKEN_TYPE::IDENTIFIER || token.m_type == TOKEN_TYPE::REFERENCE;
}

u_ptr_AstNode Parser::parseTerm(TokenList& tokenList) {
    assert(not containsFunction(tokenList));
    assert(not containsBrackets(tokenList));
    assert(not containsExpression(tokenList));
    assert(not containsFactor(tokenList));

    bool startsWithUnaryMinus = tokenList.begin()->m_type == TOKEN_TYPE::UNARY_OP;
    if (startsWithUnaryMinus) {
        tokenList.erase(tokenList.begin());
    }
    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_TERM);
    if (it == tokenList.end()) {
        assert(tokenList.size() == 1);
        if (startsWithUnaryMinus) {
            return -parseValueType(tokenList.front());
        } else {
            return parseValueType(tokenList.front());
        }
    } else {
        it = tokenList.insert(it, freshReferenceToken(0));
        it = std::next(it);
        if (std::next(it)->m_string == "+") {
            m_subExpresssionList.emplace_back((startsWithUnaryMinus ? -parseValueType(*it) : parseValueType(*it)) +
                                              parseValueType(*std::next(it, 2)));
        } else {
            m_subExpresssionList.emplace_back((startsWithUnaryMinus ? -parseValueType(*it) : parseValueType(*it)) +
                                              -parseValueType(*std::next(it, 2)));
        }
        tokenList.erase(it, std::next(it, 3));
    }
    return parseTerm(tokenList);
}

u_ptr_AstNode Parser::parseFactor(TokenList& tokenList) {
    assert(not containsFunction(tokenList));
    assert(not containsBrackets(tokenList));
    assert(not containsExpression(tokenList));

    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_FAC);
    if (it == tokenList.end()) {
        return parseTerm(tokenList);
    } else {
        it = tokenList.insert(it, freshReferenceToken(0));
        it = std::next(it);
        if (std::next(it)->m_string == "*") {
            m_subExpresssionList.emplace_back(parseValueType(*it) * parseValueType(*std::next(it, 2)));
        } else {
            assert(std::next(it)->m_string == "/");
            m_subExpresssionList.emplace_back(parseValueType(*it) / parseValueType(*std::next(it, 2)));
        }
        tokenList.erase(it, std::next(it, 3));
    }
    return parseFactor(tokenList);
}

u_ptr_AstNode Parser::parseExpression(TokenList& tokenList) {
    assert(not containsFunction(tokenList));
    assert(not containsBrackets(tokenList));

    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_EXPR);
    if (it == tokenList.end()) {
        return parseFactor(tokenList);
    } else {
        it = tokenList.insert(it, freshReferenceToken(0));
        it = std::next(it);
        m_subExpresssionList.emplace_back(parseValueType(*it) ^ parseValueType(*std::next(it, 2)));
        tokenList.erase(it, std::next(it, 3));
    }
    return parseExpression(tokenList);
}

u_ptr_AstNode Parser::parseBrackets(TokenList& tokenList) {
    assert(not containsFunction(tokenList));

    auto openBracketIt = std::find_if(tokenList.begin(), tokenList.end(),
                                      [](const Token& token) { return token.m_type == TOKEN_TYPE::LEFT_BR; });
    while (openBracketIt != tokenList.end()) {
        auto closingBracketIt =
            std::find(openBracketIt, tokenList.end(), Token{TOKEN_TYPE::RIGHT_BR, openBracketIt->m_string});
        assert(closingBracketIt != tokenList.end());

        openBracketIt    = tokenList.insert(openBracketIt, freshReferenceToken(0));
        openBracketIt    = tokenList.erase(std::next(openBracketIt));
        closingBracketIt = tokenList.erase(closingBracketIt);

        TokenList expressionInBrackets;
        expressionInBrackets.splice(expressionInBrackets.begin(), tokenList, openBracketIt, closingBracketIt);

        const size_t indexOfNewExpression = m_subExpresssionList.size();
        m_subExpresssionList.emplace_back(nullptr);
        m_subExpresssionList[indexOfNewExpression] = parseBrackets(expressionInBrackets);

        openBracketIt = std::find_if(tokenList.begin(), tokenList.end(),
                                     [](const Token& token) { return token.m_type == TOKEN_TYPE::LEFT_BR; });
    }
    return parseExpression(tokenList);
}

std::string Parser::toString(const TokenList& tokenList) const {
    std::string result;
    for (const auto& token : tokenList) {
        switch (token.m_type) {
            case TOKEN_TYPE::REFERENCE: {
                const auto indexString = token.m_string.substr(1);
                const auto index       = std::stoul(indexString);
                assert(index < m_subExpresssionList.size());
                result += m_subExpresssionList.at(index)->toString();
                break;
            }
            case TOKEN_TYPE::LEFT_BR:
                result += std::string("(");
                break;
            case TOKEN_TYPE::RIGHT_BR:
                result += std::string(")");
                break;
            default:
                result += token.m_string;
                break;
        }
    }
    return result;
}

Token Parser::freshReferenceToken(size_t offset) const {
    return Token{TOKEN_TYPE::REFERENCE, "$" + std::to_string(m_subExpresssionList.size() - offset)};
}

u_ptr_AstNode Parser::parseLeadingValueType(TokenList& tokenList) {
    u_ptr_AstNode leadingTerm;
    if (tokenList.begin()->m_type == TOKEN_TYPE::UNARY_OP) {
        tokenList.pop_front();
        assert(isValueType(tokenList.front()));
        leadingTerm = -parseValueType(*tokenList.begin());
        tokenList.pop_front();
    } else {
        assert(isValueType(tokenList.front()));
        leadingTerm = parseValueType(*tokenList.begin());
        tokenList.pop_front();
    }
    return leadingTerm;
}

u_ptr_AstNode Parser::parseFunctions(TokenList tokenList) {
    auto functionIdIt = std::find_if(tokenList.begin(), tokenList.end(),
                                     [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; });
    while (functionIdIt != tokenList.end()) {
        auto openBracketIt = std::next(functionIdIt);
        auto closingBracketIt =
            std::find(functionIdIt, tokenList.end(), Token{TOKEN_TYPE::RIGHT_BR, openBracketIt->m_string});
        assert(closingBracketIt != tokenList.end());

        const std::string functionName = functionIdIt->m_string;

        functionIdIt     = tokenList.erase(functionIdIt);
        functionIdIt     = tokenList.erase(functionIdIt);
        closingBracketIt = tokenList.erase(closingBracketIt);
        functionIdIt     = tokenList.insert(functionIdIt, freshReferenceToken(0));

        TokenList expressionInBrackets;
        expressionInBrackets.splice(expressionInBrackets.begin(), tokenList, std::next(functionIdIt), closingBracketIt);

        const size_t indexOfNewExpression = m_subExpresssionList.size();
        m_subExpresssionList.emplace_back(nullptr);
        m_subExpresssionList[indexOfNewExpression] =
            std::make_unique<AstNodeFunction>(functionName, parse(expressionInBrackets));

        functionIdIt = std::find_if(tokenList.begin(), tokenList.end(),
                                    [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; });
    }
    return parseBrackets(tokenList);
}
