//
// Created by pc on 20-11-20.
//

#include "AstNodeFunction.h"

#include <cassert>

namespace ast {
    AstNodeFunction::AstNodeFunction(std::string functionName, u_ptr_AstNode&& argument)
        : m_functionName(std::move(functionName)), m_argument(std::move(argument)) {
    }

    std::string AstNodeFunction::toString() const {
        return m_functionName + "(" + m_argument->toString() + ")";
    }

    u_ptr_AstNode AstNodeFunction::copy() const {
        return u_ptr_AstNode(new AstNodeFunction(m_functionName, m_argument->copy()));
    }

    u_ptr_AstNode AstNodeFunction::simplify() const {
        AstNode* simplifiedNode = new AstNodeFunction(m_functionName, m_argument->simplify());
        return u_ptr_AstNode(simplifiedNode);
    }

    AstNode::NODE_TYPE AstNodeFunction::type() const {
        return NODE_TYPE::FUNCTION;
    }

    bool AstNodeFunction::equals(const AstNode& other) const {
        if (other.type() == AstNode::NODE_TYPE::FUNCTION) {
            return *m_argument == *other.childAt(0);
        }
        return false;
    }

    size_t AstNodeFunction::childCount() const {
        return 1;
    }

    const AstNode* AstNodeFunction::childAt(size_t index) const {
        assert(index < childCount());
        return m_argument.get();
    }

    bool AstNodeFunction::compareEqualType(const AstNode* rhs) const {
        assert(rhs->type() == type());
        const auto rightFunctionName = dynamic_cast<const AstNodeFunction*>(rhs)->m_functionName;
        if (m_functionName != rightFunctionName) {
            return m_functionName < rightFunctionName;
        } else {
            return compare(m_argument.get(), rhs->childAt(0));
        }
    }

    u_ptr_AstNode AstNodeFunction::differentiate(const std::string& variable) const {
        return makeError();
    }

    std::set<std::string> AstNodeFunction::collectVariables() const {
        return m_argument->collectVariables();
    }
    gen::Number AstNodeFunction::eval(const std::map<std::string, gen::Number>& arguments) const {
        assert(false);
        return gen::Number(0ll);
    }
} // namespace ast