//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"

#include <cassert>

AstNodeUnaryMinus::AstNodeUnaryMinus(std::unique_ptr<AstNode>&& value) : m_value(std::move(value)) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeUnaryMinus::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeUnaryMinus(m_value->copy()));
}

std::unique_ptr<AstNode> AstNodeUnaryMinus::simplify() const {
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