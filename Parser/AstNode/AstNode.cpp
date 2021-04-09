//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include "AstNodeAdd.h"
#include "AstNodeDiv.h"
#include "AstNodeError.h"
#include "AstNodeMul.h"
#include "AstNodeNumber.h"
#include "AstNodePower.h"
#include "AstNodeSubtract.h"
#include "AstNodeUnaryMinus.h"

#include <cassert>

bool AstNode::isNumeric() const {
    return type() == NODE_TYPE::NUMBER;
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

bool AstNode::isZero() const {
    if (not isNumeric()) {
        return false;
    }
    return std::visit([](const auto& a) { return a == 0.0; }, dynamic_cast<const AstNodeNumber&>(*this).value().number());
}

bool AstNode::isOne() const {
    if (not isNumeric()) {
        return false;
    }
    return std::visit([](const auto& a) { return a == 1.0; }, dynamic_cast<const AstNodeNumber&>(*this).value().number());
}

bool AstNode::isEven() const {
    std::cout << "Implement isEven in AstNode.cpp\n";
    return false;
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
    if (val == nullptr) {
        return nullptr;
    }
    return std::make_unique<AstNodeUnaryMinus>(std::move(val));
}

u_ptr_AstNode AstNode::copy(const u_ptr_AstNode& node) {
    return node->copy();
}

u_ptr_AstNode AstNode::simplify(const u_ptr_AstNode& node) {
    return node->simplify();
}

u_ptr_AstNode AstNode::makeZeroNode() {
    return makeNumber(0ll);
}

u_ptr_AstNode AstNode::makeOneNode() {
    return makeNumber(1ll);
}

u_ptr_AstNode AstNode::makeNumber(std::variant<long long int, double> val) {
    return std::make_unique<AstNodeNumber>(val);
}

u_ptr_AstNode AstNode::makeNumber(Number val) {
    return std::make_unique<AstNodeNumber>(val);
}

Number AstNode::getNumber() const {
    assert(type() == NODE_TYPE::NUMBER);
    return dynamic_cast<const AstNodeNumber&>(*this).value();
}

u_ptr_AstNode AstNode::makeError() {
    return u_ptr_AstNode(new AstNodeError());
}
