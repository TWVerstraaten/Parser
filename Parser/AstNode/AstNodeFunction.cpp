//
// Created by pc on 20-11-20.
//

#include "AstNodeFunction.h"

#include <cassert>
#include <utility>

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
        return *m_argument == *dynamic_cast<const AstNodeFunction&>(other).m_argument;
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
    const auto* rightSide = dynamic_cast<const AstNodeFunction*>(rhs);
    if (m_functionName != rightSide->m_functionName) {
        return m_functionName < rightSide->m_functionName;
    } else {
        return compare_u_ptr(m_argument, rightSide->m_argument);
    }
}
