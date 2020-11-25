//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"

#include <cassert>
#include <iostream>

AstNodeUnaryMinus::AstNodeUnaryMinus(std::unique_ptr<AstNode>&& value) : m_value(std::move(value)) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeUnaryMinus::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeUnaryMinus(m_value->copy()));
}

std::unique_ptr<AstNode> AstNodeUnaryMinus::simplify() const {
    //    Pattern doubleMinusPattern({T::MINUS, T::CH0, T::NAME_A, T::CLOSE, T::CLOSE});
    //    if (doubleMinusPattern.match(m_value.get())) {
    //        return doubleMinusPattern.node("A")->copy()->simplify();
    //    }
    //    Pattern childIsNumeric({T::NUM, T::NAME_A, T::CLOSE});
    //    if (childIsNumeric.match(m_value.get())) {
    //        const AstNode* numericNode = childIsNumeric.node("A");
    //        if (numericNode->type() == AstNode::NODE_TYPE::DOUBLE) {
    //            return std::unique_ptr<AstNode>(
    //                new AstNodeDouble(-dynamic_cast<const AstNodeDouble*>(numericNode)->value()));
    //        } else {
    //            return std::unique_ptr<AstNode>(
    //                new AstNodeInteger(-dynamic_cast<const AstNodeInteger*>(numericNode)->value()));
    //        }
    //    }
    //
    AstNode* simplifiedNode = new AstNodeUnaryMinus(m_value->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeUnaryMinus::type() const {
    return NODE_TYPE::UNARY_MINUS;
}

bool AstNodeUnaryMinus::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::UNARY_MINUS) {
        return *m_value == *dynamic_cast<const AstNodeUnaryMinus&>(other).m_value;
    }
    return false;
}

size_t AstNodeUnaryMinus::childCount() const {
    return 1;
}

const AstNode* AstNodeUnaryMinus::childAt(size_t index) const {
    assert(index < childCount());
    return m_value.get();
}

// {* MINUS}