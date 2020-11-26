//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

#include "AstNodeNumeric.h"

#include <cassert>

AstNodeDiv::AstNodeDiv(u_ptr_AstNode&& base, u_ptr_AstNode&& exponent)
    : m_leftNode(std::move(base)), m_rightNode(std::move(exponent)) {
}

std::string AstNodeDiv::toString() const {
    return "(" + m_leftNode->toString() + " / " + m_rightNode->toString() + ")";
}

u_ptr_AstNode AstNodeDiv::copy() const {
    return u_ptr_AstNode(new AstNodeDiv(m_leftNode->copy(), m_rightNode->copy()));
}

u_ptr_AstNode AstNodeDiv::simplify() const {
    const auto left  = m_leftNode->simplify();
    const auto right = m_rightNode->simplify();
    if (left->isNumeric() && right->isNumeric()) {
        return (NUMERIC_CAST(left.get()) / NUMERIC_CAST(right.get())).toNode();
    }

    AstNode* simplifiedNode = new AstNodeDiv(m_leftNode->simplify(), m_rightNode->simplify());
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeDiv::type() const {
    return NODE_TYPE::DIVIDE;
}

bool AstNodeDiv::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::DIVIDE) {
        const auto& candidate = dynamic_cast<const AstNodeDiv&>(other);
        return (*m_leftNode == *candidate.m_leftNode && *m_rightNode == *candidate.m_rightNode);
    }
    return false;
}

size_t AstNodeDiv::childCount() const {
    return 2ul;
}

const AstNode* AstNodeDiv::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}
bool AstNodeDiv::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    const auto* rightSide = dynamic_cast<const AstNodeDiv*>(rhs);
    if (*m_leftNode == *rightSide->m_leftNode) {
        return compare_u_ptr(m_rightNode, rightSide->m_rightNode);
    } else {
        return compare_u_ptr(m_leftNode, rightSide->m_leftNode);
    }
}
