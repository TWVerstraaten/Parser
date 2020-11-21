//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"

#include <cassert>

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

bool operator==(const AstNode& lhs, const AstNode& rhs) {
    return lhs.equals(rhs);
}

bool operator<(const AstNode& lhs, const AstNode& rhs) {
    return lhs.type() < rhs.type();
}
