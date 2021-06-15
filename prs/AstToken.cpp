//
// Created by pc on 07-06-21.
//

#include "AstToken.h"

#include "../gen/Overloaded.h"
#include "../gen/defines.h"
#include "ParserInfo.h"
#include "TokenWriter.h"

#include <algorithm>
#include <cassert>

AstToken::AstToken(const std::list<StructuralToken>& structuralTokens, ParserInfo& info) {
    switch (structuralTokens.size()) {
        case 0:
            m_range = Range{};
            m_token = Empty{};
            return;
        case 1:
            m_range = structuralTokens.front().range();
            break;
        default:
            m_range = {structuralTokens.front().range().startIndex(), structuralTokens.back().range().endIndex()};
            break;
    }

    if (auto it = std::find_if(TT_IT(structuralTokens), TT_LAMBDA(a, return TokenTemplates::isTokenOfType<Token>(a.token(), Token::TYPE::EQUALS);)); it != structuralTokens.end()) {
        m_token    = OPERATOR_TYPE::EQUALS;
        m_children = {AstToken{std::list<StructuralToken>(structuralTokens.begin(), it), info}, AstToken{std::list<StructuralToken>(std::next(it), structuralTokens.end()), info}};
        return;
    }

    TempTokenList tempTokens;
    for (const auto& el : structuralTokens) {
        std::visit(Overloaded{[&](const Token& a) { tempTokens.emplace_back(a); }, [&](const auto& a) { tempTokens.emplace_back(AstToken{a, el.range(), info}); }}, el.token());
    }

    replacePowers(tempTokens, info);
    replaceTimesDivide(tempTokens, info);
    replaceUnaryMinuses(tempTokens, info);
    replacePlusMinus(tempTokens, info);

    if (tempTokens.size() > 1) {
        info.addError(ParserError{ParserError::TYPE::GENERIC, std::string("More than one token left at AstToken? ") + TT_WHERE_STRING});
    }
    assert(not tempTokens.empty());
    assert(std::holds_alternative<AstToken>(tempTokens.front()));
    m_children = std::move(std::get<AstToken>(tempTokens.front()).m_children);
    m_token    = std::get<AstToken>(tempTokens.front()).m_token;
}

AstToken::AstToken(const StructuralToken::Bracketed& bracketed, Range range, ParserInfo& info) : m_token(VectorToken{bracketed.m_tokenLists.size()}), m_range(range) {
    for (const auto& el : bracketed.m_tokenLists) {
        m_children.emplace_back(el, info);
    }
    if (m_children.size() == 1 && std::holds_alternative<Empty>(m_children.front().m_token)) {
        m_children.clear();
        std::get<VectorToken>(m_token).m_dimension = 0;
    }
}

AstToken::AstToken(const StructuralToken::Function& function, Range range, ParserInfo& info)
    : m_token(CustomFunctionToken{function.m_name, function.m_arguments.m_tokenLists.size()}), m_range(range) {
    for (const auto& el : function.m_arguments.m_tokenLists) {
        m_children.emplace_back(el, info);
    }
    if (m_children.size() == 1 && std::holds_alternative<Empty>(m_children.front().m_token)) {
        m_children.clear();
        std::get<CustomFunctionToken>(m_token).m_argumentCount = 0;
    }
    maybeCastToReservedFunction(info);
}

AstToken::AstToken(const std::string& string, Range range, ParserInfo& info) : m_token(string), m_range(range) {
}

AstToken::AstToken(long long int value, Range range, ParserInfo& info) : m_token(value), m_range(range) {
}

AstToken::AstToken(double value, Range range, ParserInfo& info) : m_token(value), m_range(range) {
}

AstToken::AstToken(AstToken::OPERATOR_TYPE type, AstToken astToken, Range range, ParserInfo& info) : m_token(type), m_children({std::move(astToken)}), m_range(range) {
}

AstToken::AstToken(AstToken::OPERATOR_TYPE type, AstToken left, AstToken right, Range range, ParserInfo& info)
    : m_token(type), m_children({std::move(left), std::move(right)}), m_range(range) {
}

void AstToken::maybeCastToReservedFunction(ParserInfo& info) {
    assert(std::holds_alternative<CustomFunctionToken>(m_token));
    if (auto reserved = ReservedFunction::getReserved(std::get<CustomFunctionToken>(m_token).m_name); reserved.has_value()) {
        const auto val                   = reserved.value();
        const auto argumentCount         = std::get<CustomFunctionToken>(m_token).m_argumentCount;
        const auto requiredArgumentCount = ReservedFunction::getArgumentCount(val);
        if (requiredArgumentCount != argumentCount) {
            info.addError({ParserError::TYPE::WRONG_ARGUMENT_COUNT_RESERVED,
                           ReservedFunction::getName(val) + " takes " + std::to_string(requiredArgumentCount) + " arguments, not " + std::to_string(argumentCount),
                           m_range});
        }
        m_token = ReservedFunction{val};
    }
}

std::string AstToken::toStringAsTree(const AstToken& root) {
    return TokenWriter::toStringAsTree("", root, false);
}

[[nodiscard]] static std::list<std::variant<AstToken, Token>>::iterator
tokenIt(std::list<std::variant<AstToken, Token>>& tempTokens, std::list<std::variant<AstToken, Token>>::iterator it, const std::set<Token::TYPE>& types) {
    return std::find_if(it, tempTokens.end(), [&](const auto& a) { return std::holds_alternative<Token>(a) && (types.find(std::get<Token>(a).type()) != types.end()); });
}

void AstToken::replaceUnaryMinuses(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::UNARY_MINUS};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, std::next(it), s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto& token = std::get<Token>(*it);
        assert(token.type() == Token::TYPE::UNARY_MINUS);
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        const size_t start = token.range().startIndex();
        it                 = tempTokens.erase(it);
        const size_t end   = std::get<AstToken>(*it).m_range.endIndex();
        *it                = AstToken{OPERATOR_TYPE::UNARY_MINUS, std::get<AstToken>(*it), Range{start, end}, info};
    }
}

void AstToken::replacePowers(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::POWER};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        assert(std::get<Token>(*it).type() == Token::TYPE::POWER);
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start = std::get<AstToken>(*std::prev(it)).m_range.startIndex();
        const size_t end   = std::get<AstToken>(*std::next(it)).m_range.endIndex();
        *std::prev(it)     = AstToken{OPERATOR_TYPE::POWER, std::move(std::get<AstToken>(*std::prev(it))), std::move(std::get<AstToken>(*std::next(it))), {start, end}, info};
        it                 = tempTokens.erase(it, std::next(it, 2));
    }
}

void AstToken::replaceTimesDivide(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::TIMES, Token::TYPE::DIVIDE};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto type = std::get<Token>(*it).type();
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start        = std::get<AstToken>(*std::prev(it)).m_range.startIndex();
        const size_t end          = std::get<AstToken>(*std::next(it)).m_range.endIndex();
        const auto   astTokenType = type == Token::TYPE::TIMES ? OPERATOR_TYPE::TIMES : OPERATOR_TYPE::DIVIDE;
        *std::prev(it)            = AstToken{astTokenType, std::move(std::get<AstToken>(*std::prev(it))), std::move(std::get<AstToken>(*std::next(it))), {start, end}, info};
        it                        = tempTokens.erase(it, std::next(it, 2));
    }
}

void AstToken::replacePlusMinus(TempTokenList& tempTokens, ParserInfo& info) {
    static const std::set<Token::TYPE> s{Token::TYPE::PLUS, Token::TYPE::MINUS};
    for (auto it = tokenIt(tempTokens, tempTokens.begin(), s); it != tempTokens.end(); it = tokenIt(tempTokens, it, s)) {
        assert(std::holds_alternative<Token>(*it));
        const auto type = std::get<Token>(*it).type();
        assert(std::holds_alternative<AstToken>(*std::next(it)));
        assert(std::holds_alternative<AstToken>(*std::prev(it)));
        const size_t start        = std::get<AstToken>(*std::prev(it)).m_range.startIndex();
        const size_t end          = std::get<AstToken>(*std::next(it)).m_range.endIndex();
        const auto   astTokenType = type == Token::TYPE::PLUS ? OPERATOR_TYPE::PLUS : OPERATOR_TYPE::MINUS;
        *std::prev(it)            = AstToken{astTokenType, std::move(std::get<AstToken>(*std::prev(it))), std::move(std::get<AstToken>(*std::next(it))), {start, end}, info};
        it                        = tempTokens.erase(it, std::next(it, 2));
    }
}

std::set<CustomFunctionToken> AstToken::dependsOn() const {
    if (std::holds_alternative<OPERATOR_TYPE>(m_token) && std::get<OPERATOR_TYPE>(m_token) == OPERATOR_TYPE::EQUALS) {
        assert(m_children.size() == 2);
        return m_children.at(1).dependsOn();
    }
    std::set<CustomFunctionToken> functionDependencies;
    if (std::holds_alternative<CustomFunctionToken>(m_token)) {
        functionDependencies.insert(std::get<CustomFunctionToken>(m_token));
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

const AstToken::AstTokenVariant& AstToken::token() const {
    return m_token;
}

const std::vector<AstToken>& AstToken::children() const {
    return m_children;
}

const Range& AstToken::range() const {
    return m_range;
}
