//
// Created by pc on 09-06-21.
//

#include "TokenWriter.h"

#include "../gen/Overloaded.h"
#include "../gen/defines.h"

#include <algorithm>
#include <cassert>
#include <sstream>

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

std::string TokenWriter::printTree(const std::string& prefix, const AstToken& node, bool isLeft) {
    std::stringstream ss;
    ss << prefix;
    ss << (isLeft ? "├── " : "└── ");

    std::visit(Overloaded{[&](AstToken::OPERATOR_TYPE type) { ss << TokenWriter::toString(type); },
                          [&](const CustomFunction& function) { ss << "Fun(" << function.argumentCount() << ")@_" << function.name(); },
                          [&](const VectorToken& vector) { ss << "Vec(" << vector.m_dimension << ")"; },
                          [&](const ReservedFunction& reservedFunction) { ss << ReservedFunction::getName(reservedFunction.m_reserved); },
                          [&](const AstToken::Empty&) { ss << "_empty_"; },
                          [&](const auto& val) { ss << val; }},
               node.m_token);
    ss << "  " << node.m_range.toString() << std::endl;

    if (node.m_children.empty()) {
        return ss.str();
    }
    for (auto it = node.m_children.begin(); it != node.m_children.end(); ++it) {
        ss << printTree(prefix + (isLeft ? "│    " : "     "), *it, std::next(it) != node.m_children.end());
    }
    return ss.str();
}
