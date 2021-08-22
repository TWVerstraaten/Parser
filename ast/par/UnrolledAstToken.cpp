//
// Created by pc on 14-06-21.
//

#include "UnrolledAstToken.h"

#include "../../gen/Overloaded.h"
#include "../../gen/VariantTemplates.h"
#include "../../gen/defines.h"
#include "AstToken.h"
#include "TokenTemplates.h"
#include "TokenWriter.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace ast::par {

    static UnrolledAstToken::UnrolledToken S_FROM_NUMBER(const gen::Number& number) {
        if (number.holdsLongLong()) {
            return number.toLongLong();
        } else {
            return number.toDouble();
        }
    }

    static const auto S_BINARY_SIMPLIFY = [](auto& children, const auto& func, auto& token) {
        assert(children.size() == 2);
        if (children.at(0).isNumeric() && children.at(1).isNumeric()) {
            gen::Number result = func(children.at(0).toNumber(), children.at(1).toNumber());
            token              = S_FROM_NUMBER(result);
            children.clear();
        }
    };

    UnrolledAstToken::UnrolledAstToken(const AstToken& astToken) {
        assert(not gen::S_VARIANT_EQUALS(astToken.token(), AstToken::OPERATOR_TYPE::EQUALS));
        static const std::map<AstToken::OPERATOR_TYPE, UnrolledToken> S_OPERATOR_TO_UNROLLED = {{AstToken::OPERATOR_TYPE::PLUS, Plus{}},
                                                                                                {AstToken::OPERATOR_TYPE::MINUS, Minus{}},
                                                                                                {AstToken::OPERATOR_TYPE::TIMES, Times{}},
                                                                                                {AstToken::OPERATOR_TYPE::DIVIDE, Divide{}},
                                                                                                {AstToken::OPERATOR_TYPE::POWER, Power{}},
                                                                                                {AstToken::OPERATOR_TYPE::UNARY_MINUS, UnaryMinus{}}};
        std::visit(Overloaded{[](const AstToken::Empty&) { assert(false); },
                              [](const AstToken::Error&) { assert(false); },
                              [](const FunctionToken&) { assert(false); },
                              [this](const AstToken::OPERATOR_TYPE& type) {
                                  assert(type != AstToken::OPERATOR_TYPE::EQUALS);
                                  assert(S_OPERATOR_TO_UNROLLED.find(type) != S_OPERATOR_TO_UNROLLED.end());
                                  m_token = S_OPERATOR_TO_UNROLLED.at(type);
                              },
                              [this](const VectorToken& token) { m_token = token; },
                              [this](const auto& token) { m_token = token; }},
                   astToken.token());
        m_children.reserve(astToken.children().size());
        for (const auto& child : astToken.children()) {
            m_children.emplace_back(child);
        }
        simplify();
        unWrap1DVectors();
    }

    std::string UnrolledAstToken::toString() const {
        return TokenWriter::S_TO_STRING(*this);
    }

    bool UnrolledAstToken::isNumeric() const {
        return std::visit(Overloaded{[](double) { return true; }, [](long long) { return true; }, [](const auto&) { return false; }}, m_token);
    }

    void UnrolledAstToken::simplify() {
        for (auto& el : m_children) {
            el.simplify();
        }
        std::visit(Overloaded{[&](const Plus& p) {
                                  S_BINARY_SIMPLIFY(
                                      m_children, [](const auto& l, const auto& r) { return l + r; }, m_token);
                              },
                              [&](const Minus& p) {
                                  S_BINARY_SIMPLIFY(
                                      m_children, [](const auto& l, const auto& r) { return l - r; }, m_token);
                              },
                              [&](const Times& p) {
                                  S_BINARY_SIMPLIFY(
                                      m_children, [](const auto& l, const auto& r) { return l * r; }, m_token);
                              },
                              [&](const Divide& p) {
                                  S_BINARY_SIMPLIFY(
                                      m_children, [](const auto& l, const auto& r) { return l / r; }, m_token);
                              },
                              [&](const Power& p) {
                                  S_BINARY_SIMPLIFY(
                                      m_children, [](const auto& l, const auto& r) { return l ^ r; }, m_token);
                              },
                              [&](const UnaryMinus& p) {
                                  assert(m_children.size() == 1);
                                  if (m_children.front().isNumeric()) {
                                      m_token = S_FROM_NUMBER(-m_children.front().toNumber());
                                      m_children.clear();
                                  }
                              },
                              [&](const ReservedToken& p) { simplifyReservedFunction(); },
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
                                         return gen::Number(std::numeric_limits<double>::quiet_NaN());
                                     }},
                          m_token);
    }

    void UnrolledAstToken::simplifyReservedFunction() {
        if (std::any_of(TT_IT(m_children), TT_LAMBDA(a, return not a.isNumeric();))) {
            return;
        }
        assert(std::holds_alternative<ReservedToken>(m_token));
        const auto& p = std::get<ReservedToken>(m_token);
        switch (GET_ARGUMENT_COUNT(p)) {
            case 1:
                m_token = EVAL(p, m_children.at(0).toDouble());
                break;
            case 2:
                m_token = EVAL(p, m_children.at(0).toDouble(), m_children.at(1).toDouble());
                break;
            default:
                assert(false);
        }
        m_children.clear();
    }

    void UnrolledAstToken::unWrap1DVectors() {
        if (std::holds_alternative<VectorToken>(m_token) && m_children.size() == 1) {
            m_token    = std::move(m_children.front().m_token);
            m_children = std::move(m_children.front().m_children);
        } else {
            for (auto& el : m_children) {
                el.unWrap1DVectors();
            }
        }
    }

    void UnrolledAstToken::setVariableInternal(const std::string& variable, const gen::Number& number) {
        if (std::holds_alternative<std::string>(m_token) && std::get<std::string>(m_token) == variable) {
            m_token = S_FROM_NUMBER(number);
        } else {
            for (auto& el : m_children) {
                el.setVariableInternal(variable, number);
            }
        }
    }

    void UnrolledAstToken::setVariableInPlace(const std::string& variable, const gen::Number& number) {
        setVariableInternal(variable, number);
        simplify();
    }

    const UnrolledAstToken::UnrolledToken& UnrolledAstToken::token() const {
        return m_token;
    }

    const std::vector<UnrolledAstToken>& UnrolledAstToken::children() const {
        return m_children;
    }

} // namespace ast::par