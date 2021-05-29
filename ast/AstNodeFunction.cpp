//
// Created by pc on 20-11-20.
//

#include "AstNodeFunction.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>

namespace ast {

    AstNodeFunction::AstNodeFunction(std::string functionName, std::vector<u_ptr_AstNode>&& arguments)
        : m_functionName(std::move(functionName)), m_arguments(std::move(arguments)) {
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
    std::set<std::string> AstNodeFunction::collectVariables() const {
        std::set<std::string> result;
        for (const auto& arg : m_arguments) {
            const auto temp = arg->collectVariables();
            result.insert(temp.begin(), temp.end());
        }
        return result;
    }
    gen::Number AstNodeFunction::eval(const std::map<std::string, gen::Number>& arguments) const {
        assert(false);
        return gen::Number(0ll);
    }
} // namespace ast
