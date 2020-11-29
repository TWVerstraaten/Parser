//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include "AstNodeAdd.h"
#include "AstNodeDiv.h"
#include "AstNodeMul.h"
#include "AstNodeNumeric.h"
#include "AstNodePower.h"
#include "AstNodeSubtract.h"
#include "AstNodeUnaryMinus.h"

bool AstNode::isNumeric() const {
    return type() == NODE_TYPE::INTEGER || type() == NODE_TYPE::DOUBLE;
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

u_ptr_AstNode operator-(u_ptr_AstNode&& lhs) {
    return std::make_unique<AstNodeUnaryMinus>(std::move(lhs));
}
