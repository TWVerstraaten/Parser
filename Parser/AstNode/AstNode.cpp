//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include "AstNodeAdd.h"
#include "AstNodeDiv.h"
#include "AstNodeInteger.h"
#include "AstNodeMul.h"
#include "AstNodePower.h"
#include "AstNodeSubtract.h"
#include "AstNodeUnaryMinus.h"
#include "IntersectStruct.h"

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

bool AstNode::isEven() const {
    if (type() != NODE_TYPE::INTEGER) {
        return false;
    }
    return gcd(NUMERIC_CAST(this), 2) != 1;
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
    return node->simplify(SIMPLIFY_RULES::NONE);
}

static IntersectStruct switchAB(IntersectStruct&& intersectStruct) {
    return {std::move(intersectStruct.m_common), std::move(intersectStruct.m_secondRemainder),
            std::move(intersectStruct.m_firstRemainder)};
}

IntersectStruct factorMultiplies(const AstNodeMul* first, const AstNodeMul* second) {
    auto decomposition = AstNodeCommutative::decompose(COMMUTATIVE_C_CAST(first), COMMUTATIVE_C_CAST(second));
    if (decomposition.m_aCapB.empty()) {
        return {nullptr, nullptr, nullptr};
    } else {
        return {
            u_ptr_AstNode(new AstNodeMul(std::move(decomposition.m_aCapB)))->simplify(AstNode::SIMPLIFY_RULES::NONE),
            u_ptr_AstNode(new AstNodeMul(std::move(decomposition.m_aMinusB)))->simplify(AstNode::SIMPLIFY_RULES::NONE),
            u_ptr_AstNode(new AstNodeMul(std::move(decomposition.m_bMinusA)))->simplify(AstNode::SIMPLIFY_RULES::NONE)};
    }
}

IntersectStruct AstNode::factorNodeAndMultiply(const AstNode* first, const AstNodeMul* second) {
    assert(second->type() == AstNode::NODE_TYPE::MULTIPLY);
    assert(first->type() != NODE_TYPE::MULTIPLY);
    const auto& secondNodes = COMMUTATIVE_C_CAST(second)->m_nodes;
    if (std::find_if(secondNodes.begin(), secondNodes.end(),
                     [&first](const u_ptr_AstNode& node) { return *node == *first; }) != secondNodes.end()) {
        auto secondCopy = second->copy();
        COMMUTATIVE_CAST(secondCopy.get())->removeNode(first);
        return {first->copy(), AstNode::one(), std::move(secondCopy)};
    } else {
        return {nullptr, nullptr, nullptr};
    }
}

std::unique_ptr<AstNodeMul> expandPower(const AstNodePower* power) {
    assert(power->childAt(1)->type() == AstNode::NODE_TYPE::INTEGER);
    assert(NUMERIC_CAST(power->childAt(1)).doubleValue() > 0);
    size_t                     exponent = static_cast<size_t>(NUMERIC_CAST(power->childAt(1)).doubleValue());
    std::vector<u_ptr_AstNode> copiedNodes;
    for (size_t i = 0; i != exponent; ++i) {
        copiedNodes.emplace_back(power->childAt(0)->copy());
    }
    return std::make_unique<AstNodeMul>(std::move(copiedNodes));
}

IntersectStruct AstNode::factorPowers(const AstNodePower* power1, const AstNodePower* power2) {
    if (NUMERIC_CAST(power1->childAt(1)).doubleValue() > 0 && NUMERIC_CAST(power2->childAt(1)).doubleValue() > 0) {
        const auto expandedPower1 = expandPower(power1);
        const auto expandedPower2 = expandPower(power2);
        return factorMultiplies(expandedPower1.get(), expandedPower2.get());
    }
    return {nullptr, nullptr, nullptr};
}

IntersectStruct AstNode::factorPowerAndMultiply(const AstNodePower* power, const AstNodeMul* multiplyNode) {
    if (power->childAt(1)->type() == AstNode::NODE_TYPE::INTEGER) {
        if (NUMERIC_CAST(power->childAt(1)).doubleValue() > 0) {
            const auto expandedPower = expandPower(power);
            return factorMultiplies(expandedPower.get(), multiplyNode);
        }
    }
    return switchAB(factorNodeAndMultiply(power, multiplyNode));
}

IntersectStruct AstNode::factor(const AstNodeMul* first, const AstNode* second) {
    switch (second->type()) {
        case AstNode::NODE_TYPE::MULTIPLY:
            return factorMultiplies(first, dynamic_cast<const AstNodeMul*>(second));
        case AstNode::NODE_TYPE::POWER:
            return switchAB(factorPowerAndMultiply(dynamic_cast<const AstNodePower*>(second), first));
        default:
            return switchAB(factorNodeAndMultiply(second, first));
    }
}

IntersectStruct AstNode::factor(const AstNodePower* first, const AstNode* second) {
    switch (second->type()) {
        case NODE_TYPE::MULTIPLY:
            return factorPowerAndMultiply(first, dynamic_cast<const AstNodeMul*>(second));
        case NODE_TYPE::POWER:
            return factorPowers(first, dynamic_cast<const AstNodePower*>(second));
        default:
            break;
    }
    return {nullptr, nullptr, nullptr};
}

IntersectStruct AstNode::factor(const AstNode* first, const AstNode* second, bool isFirstPass) {
    if (*first == *second) {
        return {first->copy(), nullptr, nullptr};
    }
    switch (first->type()) {
        case NODE_TYPE::UNARY_MINUS: {
            auto result = factor(first->childAt(0), second, true);
            return {std::move(result.m_common), -std::move(result.m_firstRemainder),
                    std::move(result.m_secondRemainder)};
        }
        case NODE_TYPE::MULTIPLY:
            return factor(dynamic_cast<const AstNodeMul*>(first), second);
        case NODE_TYPE::POWER:
            return factor(dynamic_cast<const AstNodePower*>(first), second);
        default:
            if (isFirstPass) {
                return switchAB(factor(second, first, false));
            } else {
                return {nullptr, nullptr, nullptr};
            }
    }
}

u_ptr_AstNode AstNode::zero() {
    return makeInteger(0);
}

u_ptr_AstNode AstNode::one() {
    return makeInteger(1);
}

u_ptr_AstNode AstNode::makeInteger(long long val) {
    return std::make_unique<AstNodeInteger>(val);
}
