//
// Created by pc on 07-06-21.
//

#include "AstToken.h"

#include "../gen/defines.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <utility>

// https://en.cppreference.com/w/cpp/utility/variant/visit
// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

AstToken::AstToken(const std::list<StructuralToken>& structuralTokens) {
    if (auto it = std::find_if(TT_IT(structuralTokens), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TOKEN_TYPE::EQUALS);));
        it != structuralTokens.end()) {
        m_token    = TYPE::EQUALS;
        m_children = {AstToken{std::list<StructuralToken>(structuralTokens.begin(), it)},
                      AstToken{std::list<StructuralToken>(std::next(it), structuralTokens.end())}};
        return;
    }

    std::list<std::variant<AstToken, Token>> tempTokens;

    for (const auto& el : structuralTokens) {
        std::visit(overloaded{[&](const StructuralToken::Bracketed& bracketed) {
                                  if (bracketed.m_tokenLists.size() == 1) {
                                      tempTokens.emplace_back(AstToken{bracketed.m_tokenLists.front()});
                                  } else {
                                      tempTokens.emplace_back(AstToken{bracketed, el.m_range});
                                  }
                              },
                              [&](const StructuralToken::Function& function) {
                                  tempTokens.emplace_back(AstToken{function, el.m_range});
                              },
                              [&](const std::string& string) {
                                  tempTokens.emplace_back(AstToken{string, el.m_range});
                              },
                              [&](long long val) {
                                  tempTokens.emplace_back(AstToken{val, el.m_range});
                              },
                              [&](double val) {
                                  tempTokens.emplace_back(AstToken{val, el.m_range});
                              },
                              [&](const Token& a) { tempTokens.emplace_back(a); }},
                   el.m_token);
    }

    replacePowers(tempTokens);
    replaceTimesDivide(tempTokens);
    replaceUnaryMinuses(tempTokens);
    replacePlusMinus(tempTokens);

    assert(tempTokens.size() == 1);
    assert(std::holds_alternative<AstToken>(tempTokens.front()));
    *this = std::get<AstToken>(tempTokens.front());
}

AstToken::AstToken(const StructuralToken::Bracketed& bracketed, Range range)
    : m_token(Vector{bracketed.m_tokenLists.size()}), m_range(range) {
    for (const auto& el : bracketed.m_tokenLists) {
        m_children.emplace_back(el);
    }
}

AstToken::AstToken(const StructuralToken::Function& function, Range range)
    : m_token(Function{function.m_name, function.m_arguments.m_tokenLists.size()}), m_range(range) {
    for (const auto& el : function.m_arguments.m_tokenLists) {
        m_children.emplace_back(el);
    }
}

AstToken::AstToken(const std::string& string, Range range) : m_token(string), m_range(range) {
}

AstToken::AstToken(long long int value, Range range) : m_token(value), m_range(range) {
}

AstToken::AstToken(double value, Range range) : m_token(value), m_range(range) {
}

AstToken::AstToken(AstToken::TYPE type, AstToken astToken, Range range) : m_token(type), m_children({std::move(astToken)}), m_range(range) {
}

AstToken::AstToken(AstToken::TYPE type, AstToken left, AstToken right, Range range)
    : m_token(type), m_children({left, right}), m_range(range) {
}

void AstToken::printTree(const std::string& prefix, const AstToken& node, bool isLeft) const {
    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");

    std::visit(
        overloaded{[&](TYPE type) {
                       switch (type) {
                           case TYPE::PLUS:
                               std::cout << "+" << std::endl;
                               break;
                           case TYPE::MINUS:
                               std::cout << "-" << std::endl;
                               break;
                           case TYPE::TIMES:
                               std::cout << "*" << std::endl;
                               break;
                           case TYPE::DIVIDE:
                               std::cout << "/" << std::endl;
                               break;
                           case TYPE::POWER:
                               std::cout << "^" << std::endl;
                               break;
                           case TYPE::UNARY_MINUS:
                               std::cout << "-" << std::endl;
                               break;
                           case TYPE::EQUALS:
                               std::cout << "=" << std::endl;
                               break;
                       }
                   },
                   [&](const Function& function) { std::cout << "Fun" << function.m_argumentCount << "@" << function.m_name << std::endl; },
                   [&](const Vector& vector) { std::cout << "Vec" << vector.m_argumentCount << std::endl; },
                   [&](long long val) { std::cout << val << std::endl; },
                   [&](double val) { std::cout << val << std::endl; },
                   [&](const std::string& string) { std::cout << string << std::endl; }},
        node.m_token);

    const size_t childCount = node.m_children.size();
    if (childCount == 0) {
        return;
    }
    for (size_t i = 0; i != childCount - 1; ++i) {
        printTree(prefix + (isLeft ? "│   " : "    "), node.m_children.at(i), true);
    }
    printTree(prefix + (isLeft ? "│   " : "    "), node.m_children.back(), false);
}

void AstToken::printTree() const {
    printTree("", *this, false);
}

std::list<std::variant<AstToken, Token>>::iterator AstToken::tokenIt(std::list<std::variant<AstToken, Token>>&          tempTokens,
                                                                     std::list<std::variant<AstToken, Token>>::iterator it,
                                                                     const std::set<Token::TOKEN_TYPE>&                 types) {
    return std::find_if(it,
                        tempTokens.end(),
                        TT_LAMBDA_REF(a, return std::holds_alternative<Token>(a) && types.find(std::get<Token>(a).m_type) != types.end();));
}

void AstToken::replaceUnaryMinuses(std::list<std::variant<AstToken, Token>>& tempTokens) {
    std::set<Token::TOKEN_TYPE> s{Token::TOKEN_TYPE::UNARY_MINUS};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, std::next(it), s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto& token = std::get<Token>(*it);
        assert(token.m_type == Token::TOKEN_TYPE::UNARY_MINUS);
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        const size_t start = token.m_range.m_startIndex;
        it                 = tempTokens.erase(it);
        const size_t end   = std::get<AstToken>(*it).m_range.m_endIndex;
        *it                = AstToken{TYPE::UNARY_MINUS, std::get<AstToken>(*it), Range{start, end}};
    }
}

void AstToken::replacePowers(std::list<std::variant<AstToken, Token>>& tempTokens) {
    std::set<Token::TOKEN_TYPE> s{Token::TOKEN_TYPE::POWER};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        assert(std::get<Token>(*it).m_type == Token::TOKEN_TYPE::POWER);
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start = std::get<AstToken>(*std::prev(it)).m_range.m_startIndex;
        const size_t end   = std::get<AstToken>(*std::next(it)).m_range.m_endIndex;
        *std::prev(it)     = AstToken{TYPE::POWER, std::get<AstToken>(*std::prev(it)), std::get<AstToken>(*std::next(it)), {start, end}};
        it                 = tempTokens.erase(it, std::next(it, 2));
    }
}

void AstToken::replaceTimesDivide(std::list<std::variant<AstToken, Token>>& tempTokens) {
    std::set<Token::TOKEN_TYPE> s{Token::TOKEN_TYPE::TIMES, Token::TOKEN_TYPE::DIVIDE};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto type = std::get<Token>(*it).m_type;
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start = std::get<AstToken>(*std::prev(it)).m_range.m_startIndex;
        const size_t end   = std::get<AstToken>(*std::next(it)).m_range.m_endIndex;
        *std::prev(it)     = AstToken{type == Token::TOKEN_TYPE::TIMES ? TYPE::TIMES : TYPE::DIVIDE,
                                  std::get<AstToken>(*std::prev(it)),
                                  std::get<AstToken>(*std::next(it)),
                                  {start, end}};
        it = tempTokens.erase(it, std::next(it, 2));
    }
}

void AstToken::replacePlusMinus(std::list<std::variant<AstToken, Token>>& tempTokens) {
    std::set<Token::TOKEN_TYPE> s{Token::TOKEN_TYPE::PLUS, Token::TOKEN_TYPE::MINUS};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto type = std::get<Token>(*it).m_type;
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start = std::get<AstToken>(*std::prev(it)).m_range.m_startIndex;
        const size_t end   = std::get<AstToken>(*std::next(it)).m_range.m_endIndex;
        *std::prev(it)     = AstToken{type == Token::TOKEN_TYPE::PLUS ? TYPE::PLUS : TYPE::MINUS,
                                  std::get<AstToken>(*std::prev(it)),
                                  std::get<AstToken>(*std::next(it)),
                                  {start, end}};
        it = tempTokens.erase(it, std::next(it, 2));
    }
}
