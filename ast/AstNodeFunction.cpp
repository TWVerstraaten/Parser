//
// Created by pc on 20-11-20.
//

#include "AstNodeFunction.h"

#include "../alg/BoostWrapper.h"
#include "../fml/ReservedIdentifiers.h"
#include "../fml/prs/ParserException.h"
#include "ReservedFunction1.h"
#include "ReservedFunction2.h"

#include <cassert>
#include <cmath>
#include <iterator>
#include <sstream>

namespace ast {

    AstNodeFunction::AstNodeFunction(std::string functionName, std::vector<u_ptr_AstNode>&& arguments)
        : m_functionName(std::move(functionName)), m_arguments(std::move(arguments)) {

        assert(m_functionName == alg::BoostWrapper::trim(m_functionName));

        if (fml::ReservedIdentifiers::isReservedFunctionName(m_functionName)) {
            const size_t expectedArgumentCount = fml::ReservedIdentifiers::argumentCountOfReservedFunction(m_functionName);
            if (m_arguments.size() != expectedArgumentCount) {
                throw fml::prs::ParserException(fml::prs::ParserException::PARSER_ERROR::RESERVED_FUNCTION_WRONG_ARG_COUNT,
                                                ": " + m_functionName + " should have " + std::to_string(expectedArgumentCount) +
                                                    " arguments");
            }

#define TT_RESERVED_CAST_1_(x) std::make_unique<ReservedFunction1>([](const auto& a) { return x(a); })
#define TT_RESERVED_CAST_2_(x) std::make_unique<ReservedFunction2>([](const auto& a, const auto& b) { return x(a, b); })
            if (m_functionName == "sin") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::sin);
            } else if (m_functionName == "cos") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::cos);
            } else if (m_functionName == "tan") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::tan);
            } else if (m_functionName == "exp") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::exp);
            } else if (m_functionName == "asin") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::asin);
            } else if (m_functionName == "acos") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::acos);
            } else if (m_functionName == "atan") {
                m_reservedFunction = TT_RESERVED_CAST_1_(std::atan);
            } else if (m_functionName == "atan2") {
                m_reservedFunction = TT_RESERVED_CAST_2_(std::atan2);
            } else {
                throw fml::prs::ParserException(fml::prs::ParserException::PARSER_ERROR::RESERVED_FUNCTION_NOT_IMPLEMENTED,
                                                ": " + m_functionName);
            }
#undef TT_RESERVED_CAST_1_
#undef TT_RESERVED_CAST_2_
        }
    }

    std::string AstNodeFunction::toString() const {
        if (m_arguments.empty()) {
            return "";
        }
        std::ostringstream oss;
        oss << m_functionName + "(";
        for (size_t i = 0; i != m_arguments.size(); ++i) {
            oss << m_arguments.at(i)->toString();
            if (i + 1 != m_arguments.size()) {
                oss << ", ";
            }
        }
        oss << ")";
        return oss.str();
    }

    u_ptr_AstNode AstNodeFunction::copy() const {
        std::vector<u_ptr_AstNode> arguments;
        std::for_each(m_arguments.begin(), m_arguments.end(), [&](const auto& a) { arguments.push_back(a->copy()); });

        return u_ptr_AstNode(new AstNodeFunction(m_functionName, std::move(arguments)));
    }

    u_ptr_AstNode AstNodeFunction::simplify() const {
        std::vector<u_ptr_AstNode> simplifiedArguments;
        std::for_each(m_arguments.begin(), m_arguments.end(), [&](const auto& a) { simplifiedArguments.push_back(a->simplify()); });

        return u_ptr_AstNode(new AstNodeFunction(m_functionName, std::move(simplifiedArguments)));
    }

    AstNode::NODE_TYPE AstNodeFunction::type() const {
        return NODE_TYPE::FUNCTION;
    }

    bool AstNodeFunction::equals(const AstNode& other) const {
        if (other.type() == AstNode::NODE_TYPE::FUNCTION) {
            const auto& cast = dynamic_cast<const AstNodeFunction&>(other);
            if (cast.m_functionName != m_functionName || childCount() != other.childCount()) {
                return false;
            }
            return std::equal(m_arguments.begin(), m_arguments.end(), cast.m_arguments.begin(), cast.m_arguments.end(),
                              [](const auto& a, const auto& b) { return *a == *b; });
        }
        return false;
    }

    size_t AstNodeFunction::childCount() const {
        return m_arguments.size();
    }

    const AstNode* AstNodeFunction::childAt(size_t index) const {
        assert(index < childCount());
        return m_arguments.at(index).get();
    }

    bool AstNodeFunction::compareEqualType(const AstNode* rhs) const {
        assert(rhs->type() == type());
        const auto& right = dynamic_cast<const AstNodeFunction*>(rhs);
        if (m_functionName != right->m_functionName) {
            return m_functionName < right->m_functionName;
        } else if (childCount() != right->childCount()) {
            return childCount() < right->childCount();
        } else {
            for (auto it1 = m_arguments.begin(), it2 = right->m_arguments.begin(); it1 != m_arguments.end(); ++it1, ++it2) {
                if (not(**it1 == **it2)) {
                    return **it1 < **it2;
                }
            }
        }
        return true;
    }

    u_ptr_AstNode AstNodeFunction::differentiate(const std::string& variable) const {
        return makeError();
    }

    std::set<std::string> AstNodeFunction::usedVariables() const {
        std::set<std::string> result;
        for (const auto& arg : m_arguments) {
            const auto temp = arg->usedVariables();
            result.insert(temp.begin(), temp.end());
        }
        return result;
    }

    gen::Number AstNodeFunction::eval(const std::map<std::string, gen::Number>& arguments) const {
        if (isReserved()) {
            switch (m_arguments.size()) {
                case 1:
                    return m_reservedFunction->eval(m_arguments.at(0)->eval(arguments));
                case 2:
                    return m_reservedFunction->eval(m_arguments.at(0)->eval(arguments), m_arguments.at(1)->eval(arguments));
                default:
                    assert(false);
            }
        }
        return gen::Number(0ll);
    }

    bool AstNodeFunction::isReserved() const {
        return m_reservedFunction != nullptr;
    }

    std::set<fml::FunctionSignature> AstNodeFunction::functionDependencies() const {
        std::set<fml::FunctionSignature> result;
        for (const auto& argument : m_arguments) {
            result.merge(argument->functionDependencies());
        }
        if (not isReserved()) {
            result.insert({fml::FunctionSignature{m_functionName, m_arguments.size()}});
        }
        return result;
    }
} // namespace ast
