//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include "AstNodeAdd.h"
#include "AstNodeCommutative.h"
#include "AstNodeDiv.h"
#include "AstNodeInteger.h"
#include "AstNodeMul.h"
#include "AstNodeNumeric.h"
#include "AstNodePower.h"
#include "AstNodeSubtract.h"
#include "AstNodeUnaryMinus.h"

#include <cassert>

bool AstNode::isNumeric() const {
    return type() == NODE_TYPE::INTEGER || type() == NODE_TYPE::DOUBLE;
}

bool AstNode::isCommutative() const {
    return type() == NODE_TYPE::MULTIPLY || type() == NODE_TYPE::ADD;
}

bool AstNode::compare_u_ptr(const u_ptr_AstNode& lhs, const u_ptr_AstNode& rhs) {
    return compare(lhs.get(), rhs.get());
}

bool AstNode::compare(const AstNode* lhs, const AstNode* rhs) {
    if (lhs->type() != rhs->type()) {
        return lhs->type() < rhs->type();
    }
    return lhs->compareEqualType(rhs);
}

bool AstNode::containsCopyOf(const AstNode* node) const {
    for (size_t i = 0; i != childCount(); ++i) {
        if (*childAt(i) == *node) {
            return true;
        }
    }
    return false;
}

bool AstNode::isZero() const {
    if (not isNumeric()) {
        return false;
    }
    return NUMERIC_CAST(this).doubleValue() == 0.0;
}

bool AstNode::isOne() const {
    if (not isNumeric()) {
        return false;
    }
    return NUMERIC_CAST(this).doubleValue() == 1.0;
}

std::ostream& operator<<(std::ostream& out, const AstNode& node) {
    out << node.toString();
    return out;
}

bool operator==(const AstNode& lhs, const AstNode& rhs) {
    return lhs.equals(rhs);
}

bool operator<(const AstNode& lhs, const AstNode& rhs) {
    return AstNode::compare(&lhs, &rhs);
}

u_ptr_AstNode operator+(const AstNode& lhs, const AstNode& rhs) {
    return lhs.copy() + rhs.copy();
}

u_ptr_AstNode operator+(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs) {
    return std::make_unique<AstNodeAdd>(std::move(lhs), std::move(rhs));
}

u_ptr_AstNode operator*(const AstNode& lhs, const AstNode& rhs) {
    return lhs.copy() * rhs.copy();
}

u_ptr_AstNode operator*(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs) {
    return std::make_unique<AstNodeMul>(std::move(lhs), std::move(rhs));
}

u_ptr_AstNode operator-(const AstNode& lhs, const AstNode& rhs) {
    return lhs.copy() - rhs.copy();
}

u_ptr_AstNode operator-(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs) {
    return std::make_unique<AstNodeSubtract>(std::move(lhs), std::move(rhs));
}

u_ptr_AstNode operator/(const AstNode& lhs, const AstNode& rhs) {
    return lhs.copy() / rhs.copy();
}

u_ptr_AstNode operator/(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs) {
    return std::make_unique<AstNodeDiv>(std::move(lhs), std::move(rhs));
}

u_ptr_AstNode operator^(const AstNode& lhs, const AstNode& rhs) {
    return lhs.copy() ^ rhs.copy();
}

u_ptr_AstNode operator^(u_ptr_AstNode&& lhs, u_ptr_AstNode&& rhs) {
    return std::make_unique<AstNodePower>(std::move(lhs), std::move(rhs));
}

u_ptr_AstNode operator-(const AstNode& val) {
    return -val.copy();
}

u_ptr_AstNode operator-(u_ptr_AstNode&& val) {
    return std::make_unique<AstNodeUnaryMinus>(std::move(val));
}

u_ptr_AstNode AstNode::copy(const u_ptr_AstNode& node) {
    return node->copy();
}

u_ptr_AstNode AstNode::simplify(const u_ptr_AstNode& node) {
    return node->simplify();
}

u_ptr_AstNode AstNode::stealNode(size_t index) {
    assert(false);
    return nullptr;
}

IntersectStruct factorMultiplies(const AstNode* first, const AstNode* second) {
    assert(first->type() == AstNode::NODE_TYPE::MULTIPLY && second->type() == AstNode::NODE_TYPE::MULTIPLY);
    auto decomposition = AstNodeCommutative::decompose(COMMUTATIVE_C_CAST(first), COMMUTATIVE_C_CAST(second));
    if (decomposition.m_aCapB.empty()) {
        return {nullptr, nullptr, nullptr};
    } else {
        return {u_ptr_AstNode(new AstNodeMul(std::move(decomposition.m_aCapB))),
                u_ptr_AstNode(new AstNodeMul(std::move(decomposition.m_aMinusB))),
                u_ptr_AstNode(new AstNodeMul(std::move(decomposition.m_bMinusA)))};
    }
}

IntersectStruct AstNode::factorNodeAndMultiply(const AstNode* first, const AstNode* second) {
    assert(second->type() == AstNode::NODE_TYPE::MULTIPLY);
    assert(first->type() != NODE_TYPE::MULTIPLY);
    const auto& secondNodes = COMMUTATIVE_C_CAST(second)->m_nodes;
    if (std::find_if(secondNodes.begin(), secondNodes.end(),
                     [&first](const u_ptr_AstNode& node) { return *node == *first; }) != secondNodes.end()) {
        auto secondCopy = second->copy();
        COMMUTATIVE_CAST(secondCopy.get())->removeNode(first);
        return {first->copy(), std::make_unique<AstNodeInteger>(1), std::move(secondCopy)};
    } else {
        return {nullptr, nullptr, nullptr};
    }
}

IntersectStruct AstNode::intersect(const AstNode* first, const AstNode* second) {
    if (*first == *second) {
        return {first->copy(), nullptr, nullptr};
    }
    if (first->type() == NODE_TYPE::MULTIPLY && second->type() == NODE_TYPE::MULTIPLY) {
        return factorMultiplies(first, second);
    }
    if (second->type() == NODE_TYPE::MULTIPLY) {
        assert(first->type() != NODE_TYPE::MULTIPLY);
        return factorNodeAndMultiply(first, second);
    }
    if (first->type() == NODE_TYPE::MULTIPLY) {
        assert(second->type() != NODE_TYPE::MULTIPLY);
        auto factor = intersect(second, first);
        return {std::move(factor.m_common), std::move(factor.m_secondRemainder), std::move(factor.m_firstRemainder)};
    }

    return {nullptr, nullptr, nullptr};
}
