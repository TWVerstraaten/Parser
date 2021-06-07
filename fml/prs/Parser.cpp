//
// Created by pc on 19-11-20.
//

#include "Parser.h"

#include "../../ast/AstNodeAdd.h"
#include "../../ast/AstNodeFunction.h"
#include "../../ast/AstNodeNumber.h"
#include "../../ast/AstNodeVar.h"
#include "ParserException.h"

#include <cassert>
#include <iostream>

namespace fml::prs {

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

    static bool containsFactor(const TokenList& tokenList) {
        return std::find_if(tokenList.begin(), tokenList.end(),
                            [](const Token& token) { return token.m_type == TOKEN_TYPE::BIN_OP_FAC; }) != tokenList.end();
    }

    static bool containsExpression(const TokenList& tokenList) {
        return std::find_if(tokenList.begin(), tokenList.end(),
                            [](const Token& token) { return token.m_type == TOKEN_TYPE::BIN_OP_EXPR; }) != tokenList.end();
    }

    static bool containsFunction(const TokenList& tokenList) {
        return std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; }) !=
               tokenList.end();
    }

    static bool containsBrackets(const TokenList& tokenList) {
        return std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::LEFT_BR; }) !=
               tokenList.end();
    }

    ast::u_ptr_AstNode Parser::parse(const std::string& string) {
        return Parser::parse(Tokenizer(string).tokenList());
    }

    ast::u_ptr_AstNode Parser::parse(const TokenList& tokenList) {
        Parser parser;
        return parser.parseFunctions(tokenList);
    }

    ast::u_ptr_AstNode Parser::parseValueType(const Token& token) {
        switch (token.m_type) {
            case TOKEN_TYPE::REFERENCE: {
                const auto indexString = token.m_string.substr(1);
                const auto index       = std::stoul(indexString);
                assert(index < m_subExpressionList.size());
                return std::move(m_subExpressionList.at(index));
            }
            case TOKEN_TYPE::IDENTIFIER:
                return ast::u_ptr_AstNode(new ast::AstNodeVar(token.m_string));
            case TOKEN_TYPE::INTEGER:
                return ast::u_ptr_AstNode(new ast::AstNodeNumber(token.m_string));
            default:
                throw ParserException(ParserException::PARSER_ERROR::VALUE_TYPE_READ, token.m_string);
        }
    }

    ast::u_ptr_AstNode Parser::parseTerm(TokenList& tokenList) {
        assert(not containsFunction(tokenList));
        assert(not containsBrackets(tokenList));
        assert(not containsExpression(tokenList));
        assert(not containsFactor(tokenList));

        while (true) {
            bool startsWithUnaryMinus = tokenList.begin()->m_type == TOKEN_TYPE::UNARY_OP;
            if (startsWithUnaryMinus) {
                tokenList.erase(tokenList.begin());
            }
            if (tokenList.size() == 1) {
                return startsWithUnaryMinus ? -parseValueType(tokenList.front()) : parseValueType(tokenList.front());
            }
            auto it = tokenList.cbegin();
            it      = tokenList.insert(it, freshReferenceToken());
            it      = std::next(it);
            if (std::next(std::next(it)) == tokenList.end()) {
                throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED, std::next(it)->m_string);
            }
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
        }
    }

    ast::u_ptr_AstNode Parser::parseFactor(TokenList& tokenList) {
        assert(not containsFunction(tokenList));
        assert(not containsBrackets(tokenList));
        assert(not containsExpression(tokenList));

        while (containsFactor(tokenList)) {
            if (tokenList.size() < 3) {
                throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED);
            }
            auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_FAC);
            it      = tokenList.insert(it, freshReferenceToken());
            it      = std::next(it);
            if (std::next(std::next(it)) == tokenList.end()) {
                throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED, std::next(it)->m_string);
            }
            if (std::next(it)->m_string == "*") {
                m_subExpressionList.emplace_back(parseValueType(*it) * parseValueType(*std::next(it, 2)));
            } else {
                if (std::next(it)->m_string != "/") {
                    throw ParserException(ParserException::PARSER_ERROR::INVALID_FACTOR_OP, std::next(it)->m_string);
                }
                m_subExpressionList.emplace_back(parseValueType(*it) / parseValueType(*std::next(it, 2)));
            }
            tokenList.erase(it, std::next(it, 3));
        }
        return parseTerm(tokenList);
    }

    ast::u_ptr_AstNode Parser::parseExpression(TokenList& tokenList) {
        assert(not containsFunction(tokenList));
        assert(not containsBrackets(tokenList));

        while (containsExpression(tokenList)) {
            if (tokenList.size() < 3) {
                throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED);
            }
            auto it = lastBeforeOccurrenceOfType(tokenList.begin(), tokenList.end(), TOKEN_TYPE::BIN_OP_EXPR);
            it      = tokenList.insert(it, freshReferenceToken());
            it      = std::next(it);
            assert(std::next(it)->m_string == "^");
            if (std::next(std::next(it)) == tokenList.end()) {
                throw ParserException(ParserException::PARSER_ERROR::UNTERMINATED, std::next(it)->m_string);
            }
            m_subExpressionList.emplace_back(parseValueType(*it) ^ parseValueType(*std::next(it, 2)));
            tokenList.erase(it, std::next(it, 3));
        }
        return parseFactor(tokenList);
    }

    ast::u_ptr_AstNode Parser::parseBrackets(TokenList& tokenList) {
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

    ast::u_ptr_AstNode Parser::parseFunctions(TokenList tokenList) {
        while (containsFunction(tokenList)) {
            auto functionIdIt =
                std::find_if(tokenList.begin(), tokenList.end(), [](const Token& token) { return token.m_type == TOKEN_TYPE::FUNCTION; });
            assert(functionIdIt != tokenList.cend());
            auto openBracketIt = std::next(functionIdIt);
            if (openBracketIt == tokenList.cend() || openBracketIt->m_type != TOKEN_TYPE::FUN_OPEN_BR) {
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
            std::vector<ast::u_ptr_AstNode> arguments;
            TokenList                       currentArgument;
            auto                            commaIt = std::find_if(expressionInBrackets.begin(), expressionInBrackets.end(),
                                        [](const Token& token) { return token.m_type == TOKEN_TYPE::COMMA; });
            while (commaIt != expressionInBrackets.end()) {
                currentArgument.clear();
                currentArgument.splice(currentArgument.begin(), expressionInBrackets, expressionInBrackets.begin(), commaIt);
                arguments.push_back(parse(currentArgument));
                //                std::cout << tokensToString(currentArgument) << '\n';
                assert(expressionInBrackets.front().m_type == Tokenizer::TOKEN_TYPE::COMMA);
                expressionInBrackets.pop_front();
                commaIt = std::find_if(expressionInBrackets.begin(), expressionInBrackets.end(),
                                       [](const Token& token) { return token.m_type == TOKEN_TYPE::COMMA; });
            }
            //            std::cout << tokensToString(expressionInBrackets) << '\n';
            arguments.push_back(parse(expressionInBrackets));
            m_subExpressionList[indexOfNewExpression] = std::make_unique<ast::AstNodeFunction>(functionName, std::move(arguments));
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
} // namespace fml::prs