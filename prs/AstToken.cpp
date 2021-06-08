//
// Created by pc on 07-06-21.
//

#include "AstToken.h"

#include "../gen/Overloaded.h"
#include "../gen/defines.h"
#include "ParserInfo.h"

#include <algorithm>
#include <cassert>
#include <iostream>

AstToken::AstToken(const std::list<StructuralToken>& structuralTokens, ParserInfo& info) {
    if (structuralTokens.empty()) {
        m_token = Empty{};
        return;
    }
    if (auto it = std::find_if(TT_IT(structuralTokens), TT_LAMBDA(a, return a.isRawTokenOfType(Token::TYPE::EQUALS);));
        it != structuralTokens.end()) {
        m_token    = OPERATOR_TYPE::EQUALS;
        m_range    = {structuralTokens.front().m_range.startIndex(), structuralTokens.back().m_range.endIndex()};
        m_children = {AstToken{std::list<StructuralToken>(structuralTokens.begin(), it), info},
                      AstToken{std::list<StructuralToken>(std::next(it), structuralTokens.end()), info}};
        return;
    }

    TempTokenList tempTokens;
    for (const auto& el : structuralTokens) {
        std::visit(Overloaded{[&](const StructuralToken::Bracketed& bracketed) {
                                  if (bracketed.m_tokenLists.size() == 1) {
                                      tempTokens.emplace_back(AstToken{bracketed.m_tokenLists.front(), info});
                                  } else {
                                      tempTokens.emplace_back(AstToken{bracketed, el.m_range, info});
                                  }
                              },
                              [&](const Token& a) { tempTokens.emplace_back(a); },
                              [&](const auto& a) {
                                  tempTokens.emplace_back(AstToken{a, el.m_range, info});
                              }},
                   el.m_token);
    }

    replacePowers(tempTokens, info);
    replaceTimesDivide(tempTokens, info);
    replaceUnaryMinuses(tempTokens, info);
    replacePlusMinus(tempTokens, info);

    if (tempTokens.size() != 1) {
        for (const auto& el : tempTokens) {
            std::visit(
                Overloaded{[](const AstToken& token) { token.printTree(); }, [](const auto& a) { std::cout << a.toString() << '\n'; }}, el);
        }
        info.addError(ParserError{ParserError::TYPE::GENERIC, "More than one token left at AstToken???"});
    }

    assert(std::holds_alternative<AstToken>(tempTokens.front()));
    *this = std::get<AstToken>(tempTokens.front());
}

AstToken::AstToken(const StructuralToken::Bracketed& bracketed, Range range, ParserInfo& info)
    : m_token(Vector{bracketed.m_tokenLists.size()}), m_range(range) {
    for (const auto& el : bracketed.m_tokenLists) {
        m_children.emplace_back(el, info);
    }
}

AstToken::AstToken(const StructuralToken::Function& function, Range range, ParserInfo& info)
    : m_token(CustomFunction{function.m_name, function.m_arguments.m_tokenLists.size()}), m_range(range) {
    for (const auto& el : function.m_arguments.m_tokenLists) {
        m_children.emplace_back(el, info);
    }
    auto reserved = ReservedFunction::getReserved(function.m_name);
    if (reserved.has_value()) {
        m_token = ReservedFunction{reserved.value()};
        if (ReservedFunction::getArgumentCount(reserved.value()) != function.m_arguments.m_tokenLists.size()) {
            info.addError(ParserError{ParserError::TYPE::WRONG_ARGUMENT_COUNT_RESERVED,
                                      ReservedFunction::getName(reserved.value()) + " should have " +
                                          std::to_string(ReservedFunction::getArgumentCount(reserved.value())) + " arguments",
                                      m_range});
        }
    }
}

AstToken::AstToken(const std::string& string, Range range, ParserInfo& info) : m_token(string), m_range(range) {
}

AstToken::AstToken(long long int value, Range range, ParserInfo& info) : m_token(value), m_range(range) {
}

AstToken::AstToken(double value, Range range, ParserInfo& info) : m_token(value), m_range(range) {
}

AstToken::AstToken(AstToken::OPERATOR_TYPE type, AstToken astToken, Range range, ParserInfo& info)
    : m_token(type), m_children({std::move(astToken)}), m_range(range) {
}

AstToken::AstToken(AstToken::OPERATOR_TYPE type, AstToken left, AstToken right, Range range, ParserInfo& info)
    : m_token(type), m_children({std::move(left), std::move(right)}), m_range(range) {
}

void AstToken::printTree(const std::string& prefix, const AstToken& node, bool isLeft) {
    std::cout << prefix;
    std::cout << (isLeft ? "├── " : "└── ");

    std::visit(
        Overloaded{[](OPERATOR_TYPE type) {
                       switch (type) {
                           case OPERATOR_TYPE::PLUS:
                               std::cout << "+";
                               break;
                           case OPERATOR_TYPE::MINUS:
                               std::cout << "-";
                               break;
                           case OPERATOR_TYPE::TIMES:
                               std::cout << "*";
                               break;
                           case OPERATOR_TYPE::DIVIDE:
                               std::cout << "/";
                               break;
                           case OPERATOR_TYPE::POWER:
                               std::cout << "^";
                               break;
                           case OPERATOR_TYPE::UNARY_MINUS:
                               std::cout << "-";
                               break;
                           case OPERATOR_TYPE::EQUALS:
                               std::cout << "=";
                               break;
                       }
                   },
                   [](const CustomFunction& function) { std::cout << "Fun(" << function.m_argumentCount << ")@_" << function.m_name; },
                   [](const Vector& vector) { std::cout << "Vec(" << vector.m_argumentCount << ")"; },
                   [](const ReservedFunction& reservedFunction) { std::cout << ReservedFunction::getName(reservedFunction.m_reserved); },
                   [](const Empty&) { std::cout << "_empty_"; },
                   [](const auto& val) { std::cout << val; }},
        node.m_token);
    std::cout << "  " << node.m_range.toString() << std::endl;

    const size_t childCount = node.m_children.size();
    if (childCount == 0) {
        return;
    }
    for (size_t i = 0; i != childCount - 1; ++i) {
        printTree(prefix + (isLeft ? "│    " : "     "), node.m_children.at(i), true);
    }
    printTree(prefix + (isLeft ? "│    " : "     "), node.m_children.back(), false);
}

void AstToken::printTree() const {
    printTree("", *this, false);
}

std::list<std::variant<AstToken, Token>>::iterator tokenIt(std::list<std::variant<AstToken, Token>>&          tempTokens,
                                                           std::list<std::variant<AstToken, Token>>::iterator it,
                                                           const std::set<Token::TYPE>&                       types) {
    return std::find_if(
        it,
        tempTokens.end(),
        TT_LAMBDA_REF(a, return std::holds_alternative<Token>(a) && (types.find(std::get<Token>(a).type()) != types.end());));
}

void AstToken::replaceUnaryMinuses(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::UNARY_MINUS};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, std::next(it), s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto& token = std::get<Token>(*it);
        assert(token.m_type == Token::TYPE::UNARY_MINUS);
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        const size_t start = token.m_range.startIndex();
        it                 = tempTokens.erase(it);
        const size_t end   = std::get<AstToken>(*it).m_range.endIndex();
        *it                = AstToken{OPERATOR_TYPE::UNARY_MINUS, std::get<AstToken>(*it), Range{start, end}, info};
    }
}

void AstToken::replacePowers(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::POWER};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        assert(std::get<Token>(*it).m_type == Token::TYPE::POWER);
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start = std::get<AstToken>(*std::prev(it)).m_range.startIndex();
        const size_t end   = std::get<AstToken>(*std::next(it)).m_range.endIndex();
        *std::prev(it)     = AstToken{OPERATOR_TYPE::POWER,
                                  std::move(std::get<AstToken>(*std::prev(it))),
                                  std::move(std::get<AstToken>(*std::next(it))),
                                  {start, end},
                                  info};
        it                 = tempTokens.erase(it, std::next(it, 2));
    }
}

void AstToken::replaceTimesDivide(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::TIMES, Token::TYPE::DIVIDE};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto type = std::get<Token>(*it).m_type;
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start        = std::get<AstToken>(*std::prev(it)).m_range.startIndex();
        const size_t end          = std::get<AstToken>(*std::next(it)).m_range.endIndex();
        const auto   astTokenType = type == Token::TYPE::TIMES ? OPERATOR_TYPE::TIMES : OPERATOR_TYPE::DIVIDE;
        *std::prev(it)            = AstToken{
            astTokenType, std::move(std::get<AstToken>(*std::prev(it))), std::move(std::get<AstToken>(*std::next(it))), {start, end}, info};
        it = tempTokens.erase(it, std::next(it, 2));
    }
}

void AstToken::replacePlusMinus(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::PLUS, Token::TYPE::MINUS};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto type = std::get<Token>(*it).m_type;
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start        = std::get<AstToken>(*std::prev(it)).m_range.startIndex();
        const size_t end          = std::get<AstToken>(*std::next(it)).m_range.endIndex();
        const auto   astTokenType = type == Token::TYPE::PLUS ? OPERATOR_TYPE::PLUS : OPERATOR_TYPE::MINUS;
        *std::prev(it)            = AstToken{
            astTokenType, std::move(std::get<AstToken>(*std::prev(it))), std::move(std::get<AstToken>(*std::next(it))), {start, end}, info};
        it = tempTokens.erase(it, std::next(it, 2));
    }
}

std::set<CustomFunction> AstToken::dependsOn() const {
    if (std::holds_alternative<OPERATOR_TYPE>(m_token) && std::get<OPERATOR_TYPE>(m_token) == OPERATOR_TYPE::EQUALS) {
        assert(m_children.size() == 2);
        return m_children.at(1).dependsOn();
    }
    std::set<CustomFunction> functionDependencies;
    if (std::holds_alternative<CustomFunction>(m_token)) {
        functionDependencies.insert(std::get<CustomFunction>(m_token));
    }
    for (const auto& child : m_children) {
        functionDependencies.merge(child.dependsOn());
    }
    return functionDependencies;
}

std::set<std::string> AstToken::variablesUsed() const {
    if (std::holds_alternative<OPERATOR_TYPE>(m_token) && std::get<OPERATOR_TYPE>(m_token) == OPERATOR_TYPE::EQUALS) {
        assert(m_children.size() == 2);
        return m_children.at(1).variablesUsed();
    }
    std::set<std::string> variables;
    if (std::holds_alternative<std::string>(m_token)) {
        variables.insert(std::get<std::string>(m_token));
    }
    for (const auto& child : m_children) {
        variables.merge(child.variablesUsed());
    }
    return variables;
}
