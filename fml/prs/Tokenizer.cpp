//
// Created by pc on 19-11-20.
//

#include "Tokenizer.h"

#include "../../alg/StringAlg.h"

#include <cassert>

namespace fml::prs {
    static std::string::const_iterator endOfIdentifier(std::string::const_iterator begin, const std::string::const_iterator end) {
        while (begin != end && isalnum(*begin)) {
            ++begin;
        }
        return begin;
    }

    static std::string::const_iterator endOfNumber(std::string::const_iterator begin, const std::string::const_iterator end) {
        while (begin != end && (isdigit(*begin) || *begin == '.')) {
            ++begin;
        }
        return begin;
    }

    Tokenizer::Tokenizer(const std::string& string) {
        auto                        trimmedString = alg::StringAlg::S_TRIM(string);
        bool                        minusIsUnary  = true;
        std::string::const_iterator it            = trimmedString.cbegin();

        size_t bracketDepth = 0;
        while (it != trimmedString.cend()) {
            switch (*it) {
                case '(':
                    if (m_tokens.back().m_type == TOKEN_TYPE::IDENTIFIER) {
                        m_tokens.emplace_back(Token{TOKEN_TYPE::FUN_OPEN_BR, std::to_string(bracketDepth)});
                    } else {
                        m_tokens.emplace_back(Token{TOKEN_TYPE::LEFT_BR, std::to_string(bracketDepth)});
                    }
                    bracketDepth++;
                    break;
                case ')':
                    assert(bracketDepth > 0);
                    m_tokens.emplace_back(Token{TOKEN_TYPE::RIGHT_BR, std::to_string(bracketDepth - 1)});
                    --bracketDepth;
                    break;
                case ',':
                    m_tokens.emplace_back(Token{TOKEN_TYPE::COMMA, ","});
                    break;
                case '+':
                    m_tokens.emplace_back(Token{TOKEN_TYPE::BIN_OP_TERM, "+"});
                    break;
                case '-':
                    if (minusIsUnary) {
                        m_tokens.emplace_back(Token{TOKEN_TYPE::UNARY_OP, "-"});
                    } else {
                        m_tokens.emplace_back(Token{TOKEN_TYPE::BIN_OP_TERM, "-"});
                    }
                    break;
                case '*':
                    m_tokens.emplace_back(Token{TOKEN_TYPE::BIN_OP_FAC, "*"});
                    break;
                case '/':
                    m_tokens.emplace_back(Token{TOKEN_TYPE::BIN_OP_FAC, "/"});
                    break;
                case '^':
                    m_tokens.emplace_back(Token{TOKEN_TYPE::BIN_OP_EXPR, "^"});
                    break;
                default:
                    if (isspace(*it)) {
                        break;
                    }
                    it = parseValueType(it, trimmedString.cend());
                    break;
            }
            minusIsUnary = (*it == '(');
            ++it;
        }
        findFunctionIdentifiers();
    }

    std::string::const_iterator Tokenizer::parseValueType(std::string::const_iterator it, const std::string::const_iterator end) {
        if (isdigit(*it) || *it == '.') {
            auto        endOfNumberIt = endOfNumber(it, end);
            std::string number        = std::string(it, endOfNumberIt);
            m_tokens.emplace_back(Token{TOKEN_TYPE::INTEGER, std::move(number)});
            return std::prev(endOfNumberIt);
        } else {
            assert(isalpha(*it));
            auto endOfIdentifierIt = endOfIdentifier(it, end);
            m_tokens.emplace_back(Token{TOKEN_TYPE::IDENTIFIER, std::string(it, endOfIdentifierIt)});
            return std::prev(endOfIdentifierIt);
        }
    }

    std::string Tokenizer::toString(bool readable) const {
        return toString(m_tokens, readable);
    }

    std::string Tokenizer::toString(const TokenList& tokens, bool readable) {
        std::string result;
        if (readable) {
            for (const auto& token : tokens) {
                switch (token.m_type) {
                    case TOKEN_TYPE::LEFT_BR:
                        result += std::string("(");
                        break;
                    case TOKEN_TYPE::RIGHT_BR:
                        result += std::string(")");
                        break;
                    case TOKEN_TYPE::FUN_OPEN_BR:
                        result += std::string("[");
                        break;
                    case TOKEN_TYPE::FUN_CLOSE_BR:
                        result += std::string("]");
                        break;
                    default:
                        result += token.m_string;
                        break;
                }
            }
        } else {
            for (const auto& token : tokens) {
                switch (token.m_type) {
                    case TOKEN_TYPE::UNARY_OP:
                        result += token.m_string + "_u";
                        break;
                    case TOKEN_TYPE::LEFT_BR:
                        result += std::string("(") + "_" + token.m_string;
                        break;
                    case TOKEN_TYPE::RIGHT_BR:
                        result += std::string(")") + "_" + token.m_string;
                        break;
                    case TOKEN_TYPE::FUN_OPEN_BR:
                        result += std::string("[") + "_" + token.m_string;
                        break;
                    case TOKEN_TYPE::FUN_CLOSE_BR:
                        result += std::string("]") + "_" + token.m_string;
                        break;
                    case TOKEN_TYPE::BIN_OP_EXPR:
                        result += token.m_string + "_e";
                        break;
                    case TOKEN_TYPE::BIN_OP_FAC:
                        result += token.m_string + "_f";
                        break;
                    case TOKEN_TYPE::BIN_OP_TERM:
                        result += token.m_string + "_t";
                        break;
                    case TOKEN_TYPE::FUNCTION:
                        result += token.m_string + "_fun";
                        break;
                    case TOKEN_TYPE::IDENTIFIER:
                        result += token.m_string + "_id";
                        break;
                    default:
                        result += token.m_string;
                        break;
                }
                result += " ";
            }
        }
        return result;
    }

    Tokenizer::TokenList& Tokenizer::tokenList() {
        return m_tokens;
    }

    void Tokenizer::findFunctionIdentifiers() {
        for (auto it = m_tokens.begin(); std::next(it) != m_tokens.end(); ++it) {
            if (std::next(it)->m_type == TOKEN_TYPE::FUN_OPEN_BR) {
                assert(it->m_type == TOKEN_TYPE::IDENTIFIER);
                it->m_type = TOKEN_TYPE::FUNCTION;
            }
        }
    }
} // namespace fml::prs