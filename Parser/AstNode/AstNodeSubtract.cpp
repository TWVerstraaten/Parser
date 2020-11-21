//
// Created by pc on 19-11-20.
//

#include "AstNodeSubtract.h"

#include "AstNodeInteger.h"

#include <cassert>

AstNodeSubtract::AstNodeSubtract(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeSubtract::toString() const {
    return "(" + m_leftNode->toString() + " - " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeSubtract::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeSubtract(m_leftNode->copy(), m_rightNode->copy()));
}
std::unique_ptr<AstNode> AstNodeSubtract::simplify() const {
    const auto left  = m_leftNode->simplify();
    const auto right = m_rightNode->simplify();
    if (left->isNumeric() && right->isNumeric()) {
        return doBinaryOperation(left, right, std::minus<>(), std::minus<>());
    }

    AstNode* simplifiedNode = new AstNodeSubtract(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeSubtract::type() const {
    return NODE_TYPE::SUBTRACT;
}

bool AstNodeSubtract::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::SUBTRACT) {
        const auto& candidate = dynamic_cast<const AstNodeSubtract&>(other);
        return (*m_leftNode == *candidate.m_leftNode && *m_rightNode == *candidate.m_rightNode);
    }
    return false;
}

size_t AstNodeSubtract::childCount() const {
    return 2;
}

const AstNode* AstNodeSubtract::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}
