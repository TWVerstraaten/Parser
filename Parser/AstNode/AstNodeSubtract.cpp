//
// Created by pc on 19-11-20.
//

#include "AstNodeSubtract.h"

#include "AstNodeNumeric.h"

#include <cassert>

AstNodeSubtract::AstNodeSubtract(u_ptr_AstNode&& left, u_ptr_AstNode&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeSubtract::toString() const {
    return "(" + m_leftNode->toString() + " - " + m_rightNode->toString() + ")";
}

u_ptr_AstNode AstNodeSubtract::copy() const {
    return u_ptr_AstNode(new AstNodeSubtract(m_leftNode->copy(), m_rightNode->copy()));
}

u_ptr_AstNode AstNodeSubtract::simplify(SIMPLIFY_RULES simplifyRules) const {
    const auto left  = m_leftNode->simplify(SIMPLIFY_RULES::NONE);
    const auto right = m_rightNode->simplify(SIMPLIFY_RULES::NONE);
    if (left->isNumeric() && right->isNumeric()) {
        return (NUMERIC_CAST(left.get()) - NUMERIC_CAST(right.get())).toNode();
    }

    AstNode* simplifiedNode =
        new AstNodeSubtract(m_leftNode->simplify(SIMPLIFY_RULES::NONE), m_rightNode->simplify(SIMPLIFY_RULES::NONE));
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeSubtract::type() const {
    return NODE_TYPE::SUBTRACT;
}

bool AstNodeSubtract::equals(const AstNode& other) const {
    return other.type() == AstNode::NODE_TYPE::SUBTRACT && *m_leftNode == *other.childAt(0) &&
         *m_rightNode == *other.childAt(1);
}

size_t AstNodeSubtract::childCount() const {
    return 2;
}

const AstNode* AstNodeSubtract::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}

bool AstNodeSubtract::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    if (*m_leftNode == *rhs->childAt(0)) {
        return compare(m_rightNode.get(), rhs->childAt(1));
    } else {
        return compare(m_leftNode.get(), rhs->childAt(0));
    }
}
