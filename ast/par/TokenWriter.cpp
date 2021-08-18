//
// Created by pc on 09-06-21.
//

#include "TokenWriter.h"

#include "../../alg/StringAlg.h"
#include "../../gen/Overloaded.h"
#include "../../gen/defines.h"
#include "AstToken.h"
#include "UnrolledAstToken.h"

#include <algorithm>
#include <cassert>

namespace ast::par {

    static std::string TO_STRING(const StructuralToken::Bracketed& token, const Range& range) {
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

    static std::string TO_STRING(const StructuralToken::Function& token, const Range& range) {
        std::stringstream ss;
        ss << "F" << range.toString() << token.m_name << "(";
        ss << TO_STRING(token.m_arguments, range);
        ss << ")";
        return ss.str();
    }

    static std::string TO_STRING(const std::string& string, const Range& range) {
        return "x" + range.toString() + "(" + string + ")";
    }

    static std::string TO_STRING(double val, const Range& range) {
        return "d" + range.toString() + "(" + std::to_string(val) + ")";
    }

    static std::string TO_STRING(long long int val, const Range& range) {
        return "d" + range.toString() + "(" + std::to_string(val) + ")";
    }

    static std::string TO_STRING(AstToken::OPERATOR_TYPE type) {
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

    static std::string TO_STRING_AS_TREE_IMPL(const std::string& prefix, const AstToken& node, bool isLeft) {
        std::stringstream ss;
        ss << prefix;
        ss << (isLeft ? "├── " : "└── ");

        std::visit(Overloaded{[&](AstToken::OPERATOR_TYPE type) { ss << TO_STRING(type); },
                              [&](const FunctionToken& function) { ss << "Fun(" << function.argumentCount() << ")@_" << function.name(); },
                              [&](const VectorToken& vector) { ss << "Vec(" << vector.m_dimension << ")"; },
                              [&](const ReservedToken& reservedFunction) { ss << GET_NAME(reservedFunction); },
                              [&](const AstToken::Empty&) { ss << "_empty_"; },
                              [&](const AstToken::Error&) { ss << "_error_"; },
                              [&](const auto& val) { ss << val; }},
                   node.token());
        ss << "  " << node.range().toString() << std::endl;

        if (node.children().empty()) {
            return ss.str();
        }
        for (auto it = node.children().begin(); it != node.children().end(); ++it) {
            ss << TO_STRING_AS_TREE_IMPL(prefix + (isLeft ? "│    " : "     "), *it, std::next(it) != node.children().end());
        }
        return ss.str();
    }

    std::string TokenWriter::S_TO_STRING(const Token& token, const Range& range) {
        return "t" + range.toString() + "(" + token.toString() + ")";
    }

    std::string TokenWriter::S_TO_STRING(const UnrolledAstToken& unrolledAstToken) {
        const auto childrenString = [&]() {
            return "(" + alg::str::CONCATENATE_STRINGS<UnrolledAstToken>(unrolledAstToken.children(), [](const auto& a) { return a.toString(); }) + ")";
        };
        return std::visit(
            Overloaded{
                [&](const UnrolledAstToken::Plus& p) { return "(" + unrolledAstToken.children().front().toString() + "+" + unrolledAstToken.children().back().toString() + ")"; },
                [&](const UnrolledAstToken::Minus& p) { return "(" + unrolledAstToken.children().front().toString() + "-" + unrolledAstToken.children().back().toString() + ")"; },
                [&](const UnrolledAstToken::Times& p) { return "(" + unrolledAstToken.children().front().toString() + "*" + unrolledAstToken.children().back().toString() + ")"; },
                [&](const UnrolledAstToken::Divide& p) { return "(" + unrolledAstToken.children().front().toString() + "/" + unrolledAstToken.children().back().toString() + ")"; },
                [&](const UnrolledAstToken::Power& p) { return "(" + unrolledAstToken.children().front().toString() + "^" + unrolledAstToken.children().back().toString() + ")"; },
                [&](const UnrolledAstToken::UnaryMinus& p) { return "(-" + unrolledAstToken.children().front().toString() + ")"; },
                [&](const ReservedToken& p) { return GET_NAME(p) + childrenString(); },
                [&](const VectorToken& p) { return "(" + childrenString(); },
                [](const auto& a) {
                    std::stringstream ss;
                    ss << a;
                    return ss.str();
                }},
            unrolledAstToken.token());
    }

    std::string TokenWriter::S_TO_STRING(const StructuralToken& token, const Range& range) {
        return std::visit(Overloaded{[&](const Token& a) { return S_TO_STRING(a, range); }, [&](const auto& a) { return TO_STRING(a, range); }}, token.token());
    }

    std::string TokenWriter::S_TO_STRING_AS_TREE(const AstToken& node) {
        return TO_STRING_AS_TREE_IMPL("", node, false);
    }

    std::string TokenWriter::S_TO_STRING_FLAT(const AstToken& node) {
        const auto writeChildren = [&]() { return alg::str::CONCATENATE_STRINGS<AstToken>(node.children(), [&](const auto& a) { return S_TO_STRING_FLAT(a); }); };
        return std::visit(Overloaded{[](AstToken::Error) { return std::string("_error_"); },
                                     [](AstToken::Empty) { return std::string("_empty_"); },
                                     [&](AstToken::OPERATOR_TYPE type) {
                                         if (type == AstToken::OPERATOR_TYPE::UNARY_MINUS) {
                                             return "-(" + node.children().front().toStringFlat() + ")";
                                         } else {
                                             return "(" + node.children().front().toStringFlat() + " " + TO_STRING(type) + " " + node.children().back().toStringFlat() + ")";
                                         }
                                     },
                                     [&](const FunctionToken& function) { return function.name() + "(" + writeChildren() + ")"; },
                                     [&](const ReservedToken& function) { return GET_NAME(function) + "(" + writeChildren() + ")"; },
                                     [&](const VectorToken& vectorToken) { return "(" + writeChildren() + ")"; },
                                     [](const std::string& str) { return str; },
                                     [](const auto& a) {
                                         std::stringstream ss;
                                         ss << a;
                                         return ss.str();
                                     }},
                          node.token());
    }
} // namespace ast::par