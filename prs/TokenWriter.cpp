//
// Created by pc on 09-06-21.
//

#include "TokenWriter.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"
#include "../gen/defines.h"
#include "UnrolledAstToken.h"

#include <algorithm>
#include <cassert>

std::string TokenWriter::toString(const Token& token, const Range& range) {
    return "t" + range.toString() + "(" + token.toString() + ")";
}

std::string TokenWriter::toString(const StructuralToken::Bracketed& token, const Range& range) {
    std::stringstream ss;
    ss << "Br" << range.toString() << "(";
    bool writeComma = false;
    for (const auto& list : token.m_tokenLists) {
        if (writeComma) {
            ss << ", ";
        }
        writeComma = true;
        std::for_each(TT_IT(list), [&](const auto& a) { ss << a.toString(); });
    }
    ss << ")";
    return ss.str();
}

std::string TokenWriter::toString(const StructuralToken::Function& token, const Range& range) {
    std::stringstream ss;
    ss << "F" << range.toString() << token.m_name << "(";
    ss << toString(token.m_arguments, range);
    ss << ")";
    return ss.str();
}

std::string TokenWriter::toString(const std::string& string, const Range& range) {
    return "x" + range.toString() + "(" + string + ")";
}

std::string TokenWriter::toString(double val, const Range& range) {
    return "d" + range.toString() + "(" + std::to_string(val) + ")";
}

std::string TokenWriter::toString(long long int val, const Range& range) {
    return "d" + range.toString() + "(" + std::to_string(val) + ")";
}

std::string TokenWriter::toString(AstToken::OPERATOR_TYPE type) {
    switch (type) {
        case AstToken::OPERATOR_TYPE::PLUS:
            return "+";
        case AstToken::OPERATOR_TYPE::MINUS:
            return "-";
        case AstToken::OPERATOR_TYPE::TIMES:
            return "*";
        case AstToken::OPERATOR_TYPE::DIVIDE:
            return "/";
        case AstToken::OPERATOR_TYPE::POWER:
            return "^";
        case AstToken::OPERATOR_TYPE::UNARY_MINUS:
            return "-";
        case AstToken::OPERATOR_TYPE::EQUALS:
            return "=";
    }
    assert(false);
    return "";
}

std::string TokenWriter::toStringAsTree(const std::string& prefix, const AstToken& node, bool isLeft) {
    std::stringstream ss;
    ss << prefix;
    ss << (isLeft ? "├── " : "└── ");

    std::visit(Overloaded{[&](AstToken::OPERATOR_TYPE type) { ss << TokenWriter::toString(type); },
                          [&](const CustomFunctionToken& function) { ss << "Fun(" << function.argumentCount() << ")@_" << function.name(); },
                          [&](const VectorToken& vector) { ss << "Vec(" << vector.m_dimension << ")"; },
                          [&](const ReservedFunction& reservedFunction) { ss << ReservedFunction::getName(reservedFunction.m_reserved); },
                          [&](const AstToken::Empty&) { ss << "_empty_"; },
                          [&](const auto& val) { ss << val; }},
               node.token());
    ss << "  " << node.range().toString() << std::endl;

    if (node.children().empty()) {
        return ss.str();
    }
    for (auto it = node.children().begin(); it != node.children().end(); ++it) {
        ss << toStringAsTree(prefix + (isLeft ? "│    " : "     "), *it, std::next(it) != node.children().end());
    }
    return ss.str();
}

std::string TokenWriter::toString(const UnrolledAstToken& unrolledAstToken) {
    return std::visit(
        Overloaded{
            [&](const UnrolledAstToken::Plus& p) { return "(" + unrolledAstToken.children().front().toString() + "+" + unrolledAstToken.children().back().toString() + ")"; },
            [&](const UnrolledAstToken::Minus& p) { return "(" + unrolledAstToken.children().front().toString() + "-" + unrolledAstToken.children().back().toString() + ")"; },
            [&](const UnrolledAstToken::Times& p) { return "(" + unrolledAstToken.children().front().toString() + "*" + unrolledAstToken.children().back().toString() + ")"; },
            [&](const UnrolledAstToken::Divide& p) { return "(" + unrolledAstToken.children().front().toString() + "/" + unrolledAstToken.children().back().toString() + ")"; },
            [&](const UnrolledAstToken::Power& p) { return "(" + unrolledAstToken.children().front().toString() + "^" + unrolledAstToken.children().back().toString() + ")"; },
            [&](const UnrolledAstToken::UnaryMinus& p) { return "(-" + unrolledAstToken.children().front().toString() + ")"; },
            [&](const ReservedFunction& p) {
                return ReservedFunction::getName(p.m_reserved) + "(" +
                       alg::StringAlg::concatenateStrings<UnrolledAstToken>(unrolledAstToken.children(), [](const auto& a) { return a.toString(); }) + ")";
            },
            [&](const VectorToken& p) {
                return "(" + alg::StringAlg::concatenateStrings<UnrolledAstToken>(unrolledAstToken.children(), [](const auto& a) { return a.toString(); }) + ")";
            },
            [](const auto& a) {
                std::stringstream ss;
                ss << a;
                return ss.str();
            }},
        unrolledAstToken.token());
}
