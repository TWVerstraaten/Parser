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

std::unique_ptr<AstNode>
AstNode::doBinaryOperation(const std::unique_ptr<AstNode>& left, const std::unique_ptr<AstNode>& right,
                           const std::function<long long int(long long int, long long int)>& integerFunction,
                           const std::function<double(double, double)>&                      doubleFunction) {
    assert(left->isNumeric() && right->isNumeric());

    if (left->type() == NODE_TYPE::INTEGER && right->type() == NODE_TYPE::INTEGER) {
        return create(integerFunction(integerValue(left.get()), integerValue(right.get())));
    } else {
        return create(doubleFunction(doubleValue(left.get()), doubleValue(right.get())));
    }
}

std::unique_ptr<AstNode> AstNode::create(long long int value) {
    return std::unique_ptr<AstNode>(new AstNodeInteger(value));
}

std::unique_ptr<AstNode> AstNode::create(double value) {
    return std::unique_ptr<AstNode>(new AstNodeDouble(value));
}

double AstNode::doubleValue(const AstNode* node) {
    assert(node->isNumeric());
    if (node->type() == NODE_TYPE::INTEGER) {
        return dynamic_cast<const AstNodeInteger*>(node)->value();
    } else {
        assert(node->type() == NODE_TYPE::DOUBLE);
        return dynamic_cast<const AstNodeDouble*>(node)->value();
    }
}

long long int AstNode::integerValue(const AstNode* node) {
    assert(node->type() == NODE_TYPE::INTEGER);
    return dynamic_cast<const AstNodeInteger*>(node)->value();
}

bool AstNode::operator==(const AstNode& rhs) const {
    return equals(rhs);
}

bool AstNode::compare(const std::unique_ptr<AstNode>& lhs, const std::unique_ptr<AstNode>& rhs) {
    return lhs->type() < rhs->type();
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

std::pair<const AstNode*, const AstNode*> AstNode::findViaTypeContainingCopy(AstNode::NODE_TYPE type) const {
    for (size_t i = 0; i != childCount(); ++i) {
        for (size_t j = 0; j != childCount(); ++j) {
            if (i == j) {
                continue;
            }
            if (childAt(j)->type() == type && childAt(j)->containsCopyOf(childAt(i))) {
                return {childAt(i), childAt(j)};
            }
        }
    }
    return {nullptr, nullptr};
}

std::vector<std::unique_ptr<AstNode>>::iterator AstNode::removeChild(const AstNode*) {
    assert(false);
}

void AstNode::removeIf(std::function<bool(const AstNode*)> predicate) {
    size_t i = 0;
    while (i != childCount()) {
        if (predicate(childAt(i))) {
            removeChild(childAt(i));
        } else {
            ++i;
        }
    }
}

bool AstNode::isZero() const {
    if (not isNumeric()) {
        return false;
    }
    return doubleValue(this) == 0.0;
}

bool AstNode::isOne() const {
    if (not isNumeric()) {
        return false;
    }
    return doubleValue(this) == 1.0;
}

void AstNode::removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE type) {
    std::pair<const AstNode*, const AstNode*> pair = findViaTypeContainingCopy(AstNode::NODE_TYPE::UNARY_MINUS);
    while (pair.first != nullptr) {
        removeChild(pair.first);
        removeChild(pair.second);
        pair = findViaTypeContainingCopy(AstNode::NODE_TYPE::UNARY_MINUS);
    }
}

void AstNode::addNode(AstNode* node) {
    assert(false);
}

void AstNode::transformNodeAndNodeWithSameChild(NODE_TYPE                                                      type,
                                                const std::function<AstNode*(const AstNode*, const AstNode*)>& f) {
    const auto a = findViaTypeContainingCopy(AstNode::NODE_TYPE::MULTIPLY);
    addNode(f(a.first, a.second));

    removeChild(a.first);
    removeChild(a.second);
}
