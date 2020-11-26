//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"

#include <cassert>
#include <iostream>

bool AstNode::isNumeric() const {
    return type() == NODE_TYPE::INTEGER || type() == NODE_TYPE::DOUBLE;
}

bool AstNode::operator==(const AstNode& rhs) const {
    return equals(rhs);
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

size_t AstNode::indexOfCopy(const AstNode* node) const {
    for (size_t i = 0; i != childCount(); ++i) {
        if (*childAt(i) == *node) {
            return i;
        }
    }
    return std::numeric_limits<size_t>::max();
}

const AstNode* AstNode::findViaTypeAndChild(AstNode::NODE_TYPE type, const AstNode* node) const {
    for (size_t i = 0; i != childCount(); ++i) {
        if (childAt(i)->type() == type && childAt(i)->containsCopyOf(node)) {
            return childAt(i);
        }
    }
    return nullptr;
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
