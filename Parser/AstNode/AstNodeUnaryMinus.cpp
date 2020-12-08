//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

#include "AstNodeAdd.h"
#include "AstNodeNumeric.h"

#include <cassert>

AstNodeUnaryMinus::AstNodeUnaryMinus(u_ptr_AstNode&& value) : m_value(std::move(value)) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}

u_ptr_AstNode AstNodeUnaryMinus::copy() const {
    return u_ptr_AstNode(new AstNodeUnaryMinus(m_value->copy()));
}

u_ptr_AstNode AstNodeUnaryMinus::simplify(SIMPLIFY_RULES simplifyRules) const {
    if (m_value->type() == AstNode::NODE_TYPE::UNARY_MINUS) {
        return m_value->childAt(0)->simplify(SIMPLIFY_RULES::NONE);
    }
    if (m_value->isNumeric()) {
        return (NUMERIC_CAST(m_value.get()) * -1).toNode();
    }
    auto* simplifiedNode = new AstNodeUnaryMinus(m_value->simplify(SIMPLIFY_RULES::DISTRIBUTE_MULTIPLICATION));
    if (simplifyRules == AstNode::SIMPLIFY_RULES::DISTRIBUTE_MULTIPLICATION &&
        simplifiedNode->m_value->type() == AstNode::NODE_TYPE::ADD) {
        auto* result = new AstNodeAdd{};
        for (auto& node : COMMUTATIVE_CAST(simplifiedNode->m_value.get())->m_nodes) {
            result->m_nodes.emplace_back(-std::move(node));
        }

        return u_ptr_AstNode(result);
    }
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeUnaryMinus::type() const {
    return NODE_TYPE::UNARY_MINUS;
}

bool AstNodeUnaryMinus::equals(const AstNode& other) const {
    if (other.type() != AstNode::NODE_TYPE::UNARY_MINUS) {
        return false;
    }
    return m_value == nullptr ? other.childAt(0) == nullptr : *m_value == *other.childAt(0);
}

size_t AstNodeUnaryMinus::childCount() const {
    return 1;
}

const AstNode* AstNodeUnaryMinus::childAt(size_t index) const {
    assert(index < childCount());
    return m_value.get();
}

bool AstNodeUnaryMinus::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    return compare(m_value.get(), rhs->childAt(0));
}
