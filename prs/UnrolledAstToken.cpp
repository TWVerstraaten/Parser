//
// Created by pc on 14-06-21.
//

#include "UnrolledAstToken.h"

#include "../gen/Overloaded.h"
#include "../gen/defines.h"
#include "AstToken.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <sstream>

UnrolledAstToken::UnrolledAstToken(const AstToken& astToken) {
    assert(not TokenTemplates::tokenEquals<AstToken::OPERATOR_TYPE>(astToken.m_token, AstToken::OPERATOR_TYPE::EQUALS));
    static const std::map<AstToken::OPERATOR_TYPE, UnrolledToken> operatorToUnrolled = {{AstToken::OPERATOR_TYPE::PLUS, Plus{}},
                                                                                        {AstToken::OPERATOR_TYPE::MINUS, Minus{}},
                                                                                        {AstToken::OPERATOR_TYPE::TIMES, Times{}},
                                                                                        {AstToken::OPERATOR_TYPE::DIVIDE, Divide{}},
                                                                                        {AstToken::OPERATOR_TYPE::POWER, Power{}},
                                                                                        {AstToken::OPERATOR_TYPE::UNARY_MINUS, UnaryMinus{}}};
    std::visit(Overloaded{[](const AstToken::Empty&) { assert(false); },
                          [](const CustomFunction&) { assert(false); },
                          [this](const AstToken::OPERATOR_TYPE& type) {
                              assert(type != AstToken::OPERATOR_TYPE::EQUALS);
                              assert(operatorToUnrolled.find(type) != operatorToUnrolled.end());
                              m_token = operatorToUnrolled.at(type);
                          },
                          [this](const VectorToken& token) { m_token = token; },
                          [this](const auto& token) { m_token = token; }},
               astToken.m_token);
    m_children.reserve(astToken.m_children.size());
    for (const auto& child : astToken.m_children) {
        m_children.emplace_back(child);
    }
    unWrap1DVectors();
    simplify();
}

std::string UnrolledAstToken::toString() const {
    return std::visit(Overloaded{[this](const Plus& p) { return "(" + m_children.front().toString() + "+" + m_children.back().toString() + ")"; },
                                 [this](const Minus& p) { return "(" + m_children.front().toString() + "-" + m_children.back().toString() + ")"; },
                                 [this](const Times& p) { return "(" + m_children.front().toString() + "*" + m_children.back().toString() + ")"; },
                                 [this](const Divide& p) { return "(" + m_children.front().toString() + "/" + m_children.back().toString() + ")"; },
                                 [this](const Power& p) { return "(" + m_children.front().toString() + "^" + m_children.back().toString() + ")"; },
                                 [this](const UnaryMinus& p) { return "(-" + m_children.front().toString() + ")"; },
                                 [this](const ReservedFunction& p) { return ReservedFunction::getName(p.m_reserved) + "(" + commaSeparatedChildren() + ")"; },
                                 [this](const VectorToken& p) { return "(" + commaSeparatedChildren() + ")"; },
                                 [](const auto& a) {
                                     std::stringstream ss;
                                     ss << a;
                                     return ss.str();
                                 }},
                      m_token);
}

std::string UnrolledAstToken::commaSeparatedChildren() const {
    std::stringstream ss;
    bool              firstPass = true;
    for (const auto& child : m_children) {
        if (not firstPass) {
            ss << ",";
        }
        ss << child.toString();
        firstPass = false;
    }
    return ss.str();
}

bool UnrolledAstToken::isNumeric() const {
    return std::visit(Overloaded{[](double) { return true; }, [](long long) { return true; }, [](const auto&) { return false; }}, m_token);
}

void UnrolledAstToken::simplify() {
    for (auto& el : m_children) {
        el.simplify();
    }
    static const auto binarySimplify = [](auto& children, auto f, auto& token) {
        assert(children.size() == 2);
        if (children.at(0).isNumeric() && children.at(1).isNumeric()) {
            gen::Number result = f(children.at(0).toNumber(), children.at(1).toNumber());
            token              = fromNumber(result);
            children.clear();
        }
    };
    std::visit(Overloaded{[&](const Plus& p) {
                              binarySimplify(
                                  m_children, [](const auto& l, const auto& r) { return l + r; }, m_token);
                          },
                          [&](const Minus& p) {
                              binarySimplify(
                                  m_children, [](const auto& l, const auto& r) { return l - r; }, m_token);
                          },
                          [&](const Times& p) {
                              binarySimplify(
                                  m_children, [](const auto& l, const auto& r) { return l * r; }, m_token);
                          },
                          [&](const Divide& p) {
                              binarySimplify(
                                  m_children, [](const auto& l, const auto& r) { return l / r; }, m_token);
                          },
                          [&](const Power& p) {
                              binarySimplify(
                                  m_children, [](const auto& l, const auto& r) { return l ^ r; }, m_token);
                          },
                          [&](const UnaryMinus& p) {
                              assert(m_children.size() == 1);
                              if (m_children.front().isNumeric()) {
                                  m_token = fromNumber(-m_children.front().toNumber());
                                  m_children.clear();
                              }
                          },
                          [&](const ReservedFunction& p) { simplifyFunction(); },
                          [](const auto& a) {}},
               m_token);
}

double UnrolledAstToken::toDouble() const {
    return std::visit(Overloaded{[](double d) { return d; },
                                 [](long long d) { return static_cast<double>(d); },
                                 [](const auto& a) {
                                     assert(false);
                                     return std::numeric_limits<double>::quiet_NaN();
                                 }},
                      m_token);
}

gen::Number UnrolledAstToken::toNumber() const {
    assert(isNumeric());
    return std::visit(Overloaded{[](double d) { return gen::Number(d); },
                                 [](long long d) { return gen::Number(d); },
                                 [](const auto& a) {
                                     assert(false);
                                     return gen::Number(0ll);
                                 }},
                      m_token);
}

UnrolledAstToken::UnrolledToken UnrolledAstToken::fromNumber(const gen::Number& number) {
    if (number.holdsLongLong()) {
        return number.toLongLong();
    } else {
        return number.toDouble();
    }
}

void UnrolledAstToken::simplifyFunction() {
    if (std::any_of(TT_IT(m_children), TT_LAMBDA(a, return not a.isNumeric();))) {
        return;
    }
    const auto& p = std::get<ReservedFunction>(m_token);
    switch (ReservedFunction::getArgumentCount(p.m_reserved)) {
        case 1:
            m_token = ReservedFunction::eval(p.m_reserved, m_children.at(0).toDouble());
            break;
        case 2:
            m_token = ReservedFunction::eval(p.m_reserved, m_children.at(0).toDouble(), m_children.at(1).toDouble());
            break;
        default:
            assert(false);
    }
    m_children.clear();
}

void UnrolledAstToken::unWrap1DVectors() {
    for (auto& el : m_children) {
        el.unWrap1DVectors();
    }
    if (std::holds_alternative<VectorToken>(m_token) && m_children.size() == 1) {
        m_token    = m_children.front().m_token;
        m_children = std::move(m_children.front().m_children);
    }
}
