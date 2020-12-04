//
// Created by pc on 19-11-20.
//

#include "AstNodeDiv.h"

#include "../../Algorithm/Algorithm.h"
#include "AstNodeCommutative.h"
#include "AstNodeInteger.h"
#include "AstNodeNumeric.h"

#include <cassert>

AstNodeDiv::AstNodeDiv(u_ptr_AstNode&& numerator, u_ptr_AstNode&& denominator)
    : m_numerator(std::move(numerator)), m_denominator(std::move(denominator)) {
}

std::string AstNodeDiv::toString() const {
    return "(" + m_numerator->toString() + " / " + m_denominator->toString() + ")";
}

u_ptr_AstNode AstNodeDiv::copy() const {
    return u_ptr_AstNode(new AstNodeDiv(m_numerator->copy(), m_denominator->copy()));
}

u_ptr_AstNode AstNodeDiv::simplify() const {
    auto numerator   = m_numerator->simplify();
    auto denominator = m_denominator->simplify();
    if (denominator->isOne()) {
        return numerator;
    }
    if (numerator->isNumeric() && denominator->isNumeric()) {
        if (numerator->type() == AstNode::NODE_TYPE::INTEGER && denominator->type() == AstNode::NODE_TYPE::INTEGER) {
            const long long divisor = gcd(NUMERIC_CAST(numerator.get()), NUMERIC_CAST(denominator.get()));
            if (divisor == 1) {
                return u_ptr_AstNode(new AstNodeDiv(std::move(numerator), std::move(denominator)));
            }
            return u_ptr_AstNode(new AstNodeDiv((NUMERIC_CAST(numerator.get()) / divisor).toNode(),
                                                (NUMERIC_CAST(denominator.get()) / divisor).toNode()))
                ->simplify();
        } else {
            return (NUMERIC_CAST(numerator.get()) / NUMERIC_CAST(denominator.get())).toNode();
        }
    }

    auto commonFactorStruct = intersect(m_numerator.get(), m_denominator.get());
    if (commonFactorStruct.m_common != nullptr) {
        return u_ptr_AstNode(new AstNodeDiv(commonFactorStruct.firstOr(std::make_unique<AstNodeInteger>(1)),
                                            commonFactorStruct.secondOr(std::make_unique<AstNodeInteger>(1))))
            ->simplify();
    }

    AstNode* simplifiedNode = new AstNodeDiv(std::move(numerator), std::move(denominator));
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeDiv::type() const {
    return NODE_TYPE::DIVIDE;
}

bool AstNodeDiv::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::DIVIDE) {
        assert(other.childCount() == 2);
        return (*m_numerator == *other.childAt(0) && *m_denominator == *other.childAt(1));
    }
    return false;
}

size_t AstNodeDiv::childCount() const {
    return 2ul;
}

const AstNode* AstNodeDiv::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_numerator.get() : m_denominator.get();
}

bool AstNodeDiv::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    if (*m_numerator == *rhs->childAt(0)) {
        return compare(m_denominator.get(), rhs->childAt(1));
    } else {
        return compare(m_numerator.get(), childAt(0));
    }
}
