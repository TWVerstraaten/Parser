//
// Created by pc on 06-06-21.
//

#include "Tokenizer.h"

#include "../../gen/defines.h"
#include "../err/ParserInfo.h"

#include <algorithm>
#include <map>
#include <set>
#include <sstream>

namespace ast::par {

    Tokenizer::Tokenizer(std::string string, err::ParserInfo& info) : m_string(std::move(string)), m_info(info) {
        findIllegalCharacters();
        tokenize();
        if (not m_info.success()) {
            return;
        }
        checkBrackets();
        checkDoubleEquals();
        checkRepeatedCommas();
        checkRepeatedOperators();
        replaceUnaryMinuses();
        checkIdentifierNumberPatternWithNoSpace();
    }

    const std::list<Token>& Tokenizer::tokenList() const {
        return m_tokenList;
    }

    void Tokenizer::findIllegalCharacters() {
        static const std::string ALLOWED_SPECIAL_CHARACTERS = ".,()+-*/^=\t ";
        for (size_t i = 0; i != m_string.size(); ++i) {
            const char c = m_string.at(i);
            if (not isalnum(c) && ALLOWED_SPECIAL_CHARACTERS.find_first_of(c) == std::string::npos) {
                m_info.addError({err::ParserError::TYPE::ILLEGAL_CHARACTER, std::string(1, c), {i, i}});
            }
        }
    }

    void Tokenizer::tokenize() {
        if (not m_info.success()) {
            return;
        }
        for (size_t i = 0; i != m_string.size(); ++i) {
            const char c = m_string.at(i);
            switch (c) {
                case '(':
                    m_tokenList.emplace_back(Token{Token::TYPE::LEFT_BR, "(", {i, i}});
                    break;
                case ')':
                    m_tokenList.emplace_back(Token{Token::TYPE::RIGHT_BR, ")", {i, i}});
                    break;
                case '+':
                    m_tokenList.emplace_back(Token{Token::TYPE::PLUS, "+", {i, i}});
                    break;
                case '-':
                    m_tokenList.emplace_back(Token{Token::TYPE::MINUS, "-", {i, i}});
                    break;
                case '*':
                    m_tokenList.emplace_back(Token{Token::TYPE::TIMES, "*", {i, i}});
                    break;
                case '/':
                    m_tokenList.emplace_back(Token{Token::TYPE::DIVIDE, "/", {i, i}});
                    break;
                case '^':
                    m_tokenList.emplace_back(Token{Token::TYPE::POWER, "^", {i, i}});
                    break;
                case ',':
                    m_tokenList.emplace_back(Token{Token::TYPE::COMMA, ",", {i, i}});
                    break;
                case '=':
                    m_tokenList.emplace_back(Token{Token::TYPE::EQUALS, "=", {i, i}});
                    break;
                default:
                    if (isalpha(c)) {
                        size_t j = i + 1;
                        while (j < m_string.size() && isalnum(m_string.at(j))) {
                            ++j;
                        }
                        m_tokenList.emplace_back(Token{Token::TYPE::IDENTIFIER, m_string.substr(i, j - i), {i, j - 1}});
                        i = j - 1;
                    } else if (isdigit(c) || c == '.') {
                        size_t j = i + 1;
                        while (j < m_string.size() && (isdigit(m_string.at(j)) || m_string.at(j) == '.')) {
                            ++j;
                        }
                        m_tokenList.emplace_back(Token{Token::TYPE::NUMBER, m_string.substr(i, j - i), {i, j - 1}});
                        i = j - 1;
                    } else {
                        if (not isspace(c)) {
                            m_info.addError({err::ParserError::TYPE::GENERIC, std::string(1, c), {i, i}});
                        }
                    }
                    break;
            }
        }
    }

    void Tokenizer::replaceUnaryMinuses() {
        bool nextMinusIsUnary = true;
        for (auto& token : m_tokenList) {
            if (token.type() == Token::TYPE::MINUS && nextMinusIsUnary) {
                token.setType(Token::TYPE::UNARY_MINUS);
            } else
                switch (token.type()) {
                    case Token::TYPE::LEFT_BR:
                    case Token::TYPE::COMMA:
                    case Token::TYPE::EQUALS:
                        nextMinusIsUnary = true;
                        break;
                    default:
                        nextMinusIsUnary = false;
                        break;
                }
        }
    }

    std::string Tokenizer::toString() const {
        std::stringstream ss;
        for (const auto& token : m_tokenList) {
            ss << token.toString();
        }
        ss << '\n';
        return ss.str();
    }

    void Tokenizer::checkBrackets() {
        size_t bracketDepth = 0;
        for (auto& token : m_tokenList) {
            if (token.type() == Token::TYPE::LEFT_BR) {
                ++bracketDepth;
            } else if (token.type() == Token::TYPE::RIGHT_BR) {
                if (bracketDepth == 0) {
                    m_info.addError({err::ParserError::TYPE::UNMATCHED_CLOSING_BR, "", token.range()});
                    return;
                }
                --bracketDepth;
            }
        }
        bracketDepth = 0;
        for (auto it = m_tokenList.rbegin(); it != m_tokenList.rend(); ++it) {
            if (it->type() == Token::TYPE::RIGHT_BR) {
                ++bracketDepth;
            } else if (it->type() == Token::TYPE::LEFT_BR) {
                if (bracketDepth == 0) {
                    m_info.addError({err::ParserError::TYPE::UNMATCHED_OPEN_BR, "", it->range()});
                    return;
                }
                --bracketDepth;
            }
        }
    }

    void Tokenizer::checkDoubleEquals() {
        if (auto it = std::find_if(TT_IT(m_tokenList), TT_LAMBDA(a, return a.type() == Token::TYPE::EQUALS;)); it == m_tokenList.end()) {
            return;
        } else if (it = std::find_if(std::next(it), m_tokenList.end(), TT_LAMBDA(a, return a.type() == Token::TYPE::EQUALS;)); it != m_tokenList.end()) {
            m_info.addError({err::ParserError::TYPE::TOO_MANY_EQUALS, "", it->range()});
        }
    }

    void Tokenizer::checkRepeatedOperators() {
        if (m_tokenList.size() < 2) {
            return;
        }
        using T = Token::TYPE;
        static const std::set<T> OPERATOR_TYPES{T::POWER, T::PLUS, T::MINUS, T::TIMES, T::DIVIDE, T::UNARY_MINUS};
        static const std::set<T> REQUIRED_AFTER_OPERATORS{T::IDENTIFIER, T::NUMBER, Token::TYPE::LEFT_BR};

        for (auto it = m_tokenList.begin(); it != m_tokenList.end(); ++it) {
            if (OPERATOR_TYPES.find(it->type()) != OPERATOR_TYPES.end()) {
                if (std::next(it) == m_tokenList.end()) {
                    m_info.addError({err::ParserError::TYPE::UNFINISHED, it->string(), it->range()});
                }
                auto next = std::next(it);
                if (REQUIRED_AFTER_OPERATORS.find(next->type()) == REQUIRED_AFTER_OPERATORS.end()) {
                    m_info.addError({err::ParserError::TYPE::ILLEGAL_SEQUENCE, it->string() + " " + next->string(), {it->range().startIndex(), next->range().endIndex()}});
                }
            }
        }
    }

    void Tokenizer::checkRepeatedCommas() {
        if (m_tokenList.size() < 2) {
            return;
        }
        for (auto it = m_tokenList.begin(); std::next(it) != m_tokenList.end(); ++it) {
            if (it->type() == Token::TYPE::COMMA && std::next(it)->type() == Token::TYPE::COMMA) {
                m_info.addError({err::ParserError::TYPE::ILLEGAL_SEQUENCE, ", ,", {it->range().startIndex(), std::next(it)->range().endIndex()}});
            }
        }
    }

    void Tokenizer::checkIdentifierNumberPatternWithNoSpace() {
        if (m_tokenList.size() < 2) {
            return;
        }
        for (auto it = m_tokenList.begin(); std::next(it) != m_tokenList.end(); ++it) {
            auto next = std::next(it);
            if (it->type() == Token::TYPE::IDENTIFIER && next->type() == Token::TYPE::NUMBER) {
                m_info.addWarning(
                    {err::ParserWarning::TYPE::SUSPICIOUS_IDENTIFIER_NUM_PATTERN, it->string() + "*" + next->string(), {it->range().startIndex(), next->range().endIndex()}});
            }
        }
    }
} // namespace ast::par