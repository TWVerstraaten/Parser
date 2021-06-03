//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

#include "AstNodeCommutative.h"
#include "AstNodeError.h"

#include <cassert>

namespace ast {
    AstNodeDiv::AstNodeDiv(u_ptr_AstNode&& numerator, u_ptr_AstNode&& denominator)
        : m_numerator(std::move(numerator)), m_denominator(std::move(denominator)) {
    }

    std::string AstNodeDiv::toString() const {
        return "(" + m_numerator->toString() + " / " + m_denominator->toString() + ")";
    }

    u_ptr_AstNode AstNodeDiv::copy() const {
        return std::make_unique<AstNodeDiv>(m_numerator->copy(), m_denominator->copy());
    }

    u_ptr_AstNode AstNodeDiv::simplify() const {
        auto node = std::make_unique<AstNodeDiv>(m_numerator->simplify(), m_denominator->simplify());
        if (node->m_numerator->isZero()) {
            return node->m_denominator->isZero() ? u_ptr_AstNode(new AstNodeError{AstNodeError::ERROR_TYPE::DIVISION_BY_ZERO})
                                                 : AstNode::makeZeroNode();
        }
        if (node->m_denominator->isOne()) {
            return std::move(node->m_numerator);
        }
        if (node->m_numerator->isNumeric() && node->m_denominator->isNumeric()) {
            return makeNumber(node->m_numerator->getNumber() / node->m_denominator->getNumber());
        }
        return node;
    }

    AstNode::NODE_TYPE AstNodeDiv::type() const {
        return NODE_TYPE::DIVIDE;
    }

    bool AstNodeDiv::equals(const AstNode& other) const {
        if (other.type() == AstNode::NODE_TYPE::DIVIDE) {
            assert(other.childCount() == 2);
            return (*m_numerator == *other.childAt(0) && *m_denominator == *other.childAt(1));
        }
        return false;
    }

    size_t AstNodeDiv::childCount() const {
        return 2ul;
    }

    const AstNode* AstNodeDiv::childAt(size_t index) const {
        assert(index < childCount());
        return index == 0 ? m_numerator.get() : m_denominator.get();
    }

    bool AstNodeDiv::compareEqualType(const AstNode* rhs) const {
        assert(rhs->type() == type());
        if (*m_numerator == *rhs->childAt(0)) {
            return compare(m_denominator.get(), rhs->childAt(1));
        } else {
            return compare(m_numerator.get(), childAt(0));
        }
    }

    u_ptr_AstNode AstNodeDiv::differentiate(const std::string& variable) const {
        return makeError();
    }

    std::set<std::string> AstNodeDiv::usedVariables() const {
        auto result = m_numerator->usedVariables();
        result.merge(m_denominator->usedVariables());
        return result;
    }

    gen::Number AstNodeDiv::eval(const std::map<std::string, gen::Number>& arguments) const {
        return m_numerator->eval(arguments) / m_denominator->eval(arguments);
    }

    std::set<FunctionSignature> AstNodeDiv::functionDependencies() const {
        auto result = m_numerator->functionDependencies();
        result.merge(m_denominator->functionDependencies());
        return result;
    }
} // namespace ast