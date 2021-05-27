//
// Created by pc on 19-11-20.
//

#include "Parser.h"

#include "AstNode/AstNodeAdd.h"
#include "AstNode/AstNodeFunction.h"
#include "AstNode/AstNodeNumber.h"
#include "AstNode/AstNodeVar.h"
#include "ParserException.h"

#include <cassert>

using TokenList  = Tokenizer::TokenList;
using Token      = Tokenizer::Token;
using TOKEN_TYPE = Tokenizer::TOKEN_TYPE;

static TokenList::const_iterator lastBeforeOccurrenceOfType(TokenList::const_iterator begin, const TokenList::const_iterator end,
                                                            const TOKEN_TYPE& type) {
    while (std::next(begin) != end) {
        const auto next = std::next(begin);
        if (next->m_type == type) {
            return begin;
        }
        begin = std::next(begin);
    }
    return end;
}

static std::set<char> findIllegalCharacters(const std::string& string) {
    std::set<char>           illegalCharacters;
    static const std::string allowedSpecialCharacters = "()+-*/^=\t ";
    for (char c : string) {
        if (not isalnum(c) && allowedSpecialCharacters.find_first_of(c) == std::string::npos) {
            illegalCharacters.insert(c);
        }
    }
    return illegalCharacters;
}

static bool containsFactor(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::BIN_OP_FAC; }) !=
           tokenList.end();
}

static bool containsExpression(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::BIN_OP_EXPR; }) !=
           tokenList.end();
}

static bool containsFunction(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; }) !=
           tokenList.end();
}

static bool containsBrackets(const TokenList& tokenList) {
    return std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::LEFT_BR; }) !=
           tokenList.end();
}

u_ptr_AstNode Parser::parse(const std::string& string) {
    if (auto illegalCharacters = findIllegalCharacters(string); not illegalCharacters.empty()) {
        std::string chars;
        std::for_each(illegalCharacters.begin(), illegalCharacters.end(), [&](char c) { chars += c; });
        throw ParserException(ParserException::PARSER_ERROR::ILLEGAL_SYMBOL, chars);
    }
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
            assert(index < m_subExpressionList.size());
            return std::move(m_subExpressionList.at(index));
        }
        case TOKEN_TYPE::IDENTIFIER:
            return u_ptr_AstNode(new AstNodeVar(token.m_string));
        case TOKEN_TYPE::INTEGER:
            return u_ptr_AstNode(new AstNodeNumber(token.m_string));
        default:
            throw ParserException(ParserException::PARSER_ERROR::VALUE_TYPE_READ, token.m_string);
    }
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
    if (tokenList.size() == 1) {
        return startsWithUnaryMinus ? -parseValueType(tokenList.front()) : parseValueType(tokenList.front());
    }
    if (tokenList.size() < 3) {
        throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED);
    }
    auto it = tokenList.cbegin();
    it      = tokenList.insert(it, freshReferenceToken());
    it      = std::next(it);
    if (std::next(it)->m_string == "+") {
        m_subExpressionList.emplace_back((startsWithUnaryMinus ? -parseValueType(*it) : parseValueType(*it)) +
                                         parseValueType(*std::next(it, 2)));
    } else {
        if (std::next(it)->m_string != "-") {
            throw ParserException(ParserException::PARSER_ERROR::INVALID_TERM_OP, std::next(it)->m_string);
        }
        m_subExpressionList.emplace_back((startsWithUnaryMinus ? -parseValueType(*it) : parseValueType(*it)) +
                                         -parseValueType(*std::next(it, 2)));
    }
    tokenList.erase(it, std::next(it, 3));
    return parseTerm(tokenList);
}

u_ptr_AstNode Parser::parseFactor(TokenList& tokenList) {
    assert(not containsFunction(tokenList));
    assert(not containsBrackets(tokenList));
    assert(not containsExpression(tokenList));

    if (not containsFactor(tokenList)) {
        return parseTerm(tokenList);
    }
    if (tokenList.size() < 3) {
        throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED);
    }
    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_FAC);
    it      = tokenList.insert(it, freshReferenceToken());
    it      = std::next(it);
    if (std::next(it)->m_string == "*") {
        m_subExpressionList.emplace_back(parseValueType(*it) * parseValueType(*std::next(it, 2)));
    } else {
        if (std::next(it)->m_string != "/") {
            throw ParserException(ParserException::PARSER_ERROR::INVALID_FACTOR_OP, std::next(it)->m_string);
        }
        m_subExpressionList.emplace_back(parseValueType(*it) / parseValueType(*std::next(it, 2)));
    }
    tokenList.erase(it, std::next(it, 3));
    return parseFactor(tokenList);
}

u_ptr_AstNode Parser::parseExpression(TokenList& tokenList) {
    assert(not containsFunction(tokenList));
    assert(not containsBrackets(tokenList));

    if (not containsExpression(tokenList)) {
        return parseFactor(tokenList);
    }
    if (tokenList.size() < 3) {
        throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED);
    }
    auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_EXPR);
    it      = tokenList.insert(it, freshReferenceToken());
    it      = std::next(it);
    if (std::next(it)->m_string != "^") {
        throw ParserException(ParserException::PARSER_ERROR::INVALID_EXPR_OP, std::next(it)->m_string);
    }
    m_subExpressionList.emplace_back(parseValueType(*it) ^ parseValueType(*std::next(it, 2)));
    tokenList.erase(it, std::next(it, 3));
    return parseExpression(tokenList);
}

u_ptr_AstNode Parser::parseBrackets(TokenList& tokenList) {
    assert(not containsFunction(tokenList));
    while (containsBrackets(tokenList)) {
        auto openBracketIt =
            std::find_if(tokenList.begin(), tokenList.end(), [](const auto& token) { return token.m_type == TOKEN_TYPE::LEFT_BR; });
        if (openBracketIt == tokenList.cend()) {
            throw ParserException(ParserException::PARSER_ERROR::BRACKET_MISMATCH);
        }
        const auto openBracketString = openBracketIt->m_string;
        auto       closingBracketIt  = std::find(openBracketIt, tokenList.end(), Token{TOKEN_TYPE::RIGHT_BR, openBracketString});
        if (closingBracketIt == tokenList.cend()) {
            throw ParserException(ParserException::PARSER_ERROR::BRACKET_MISMATCH);
        }

        openBracketIt    = tokenList.insert(openBracketIt, freshReferenceToken());
        openBracketIt    = tokenList.erase(std::next(openBracketIt));
        closingBracketIt = tokenList.erase(closingBracketIt);

        TokenList expressionInBrackets;
        expressionInBrackets.splice(expressionInBrackets.begin(), tokenList, openBracketIt, closingBracketIt);

        const size_t indexOfNewExpression = m_subExpressionList.size();
        m_subExpressionList.emplace_back(nullptr);
        m_subExpressionList[indexOfNewExpression] = parseBrackets(expressionInBrackets);
    }
    return parseExpression(tokenList);
}

u_ptr_AstNode Parser::parseFunctions(TokenList tokenList) {
    while (containsFunction(tokenList)) {
        auto functionIdIt =
            std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; });
        assert(functionIdIt != tokenList.cend());
        auto openBracketIt = std::next(functionIdIt);
        if (openBracketIt == tokenList.cend()) {
            throw ParserException(ParserException::PARSER_ERROR::BRACKET_MISMATCH);
        }
        auto closingBracketIt = std::find(functionIdIt, tokenList.end(), Token{TOKEN_TYPE::RIGHT_BR, openBracketIt->m_string});
        if (closingBracketIt == tokenList.end()) {
            throw ParserException(ParserException::PARSER_ERROR::BRACKET_MISMATCH);
        }

        const auto functionName = functionIdIt->m_string;
        functionIdIt            = tokenList.erase(functionIdIt);
        functionIdIt            = tokenList.erase(functionIdIt);
        closingBracketIt        = tokenList.erase(closingBracketIt);
        functionIdIt            = tokenList.insert(functionIdIt, freshReferenceToken());

        TokenList expressionInBrackets;
        expressionInBrackets.splice(expressionInBrackets.begin(), tokenList, std::next(functionIdIt), closingBracketIt);

        const size_t indexOfNewExpression = m_subExpressionList.size();
        m_subExpressionList.emplace_back(nullptr);
        m_subExpressionList[indexOfNewExpression] = std::make_unique<AstNodeFunction>(functionName, parse(expressionInBrackets));
    }
    return parseBrackets(tokenList);
}

Token Parser::freshReferenceToken() const {
    return Token{TOKEN_TYPE::REFERENCE, "$" + std::to_string(m_subExpressionList.size())};
}

std::string Parser::toString(const TokenList& tokenList) const {
    std::string result;
    for (const auto& token : tokenList) {
        switch (token.m_type) {
            case TOKEN_TYPE::REFERENCE: {
                const auto indexString = token.m_string.substr(1);
                const auto index       = std::stoul(indexString);
                assert(index < m_subExpressionList.size());
                result += m_subExpressionList.at(index)->toString();
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
