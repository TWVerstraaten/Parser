//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"

#include <memory>

AstNodeAdd::AstNodeAdd(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeAdd::toString() const {
    return "(" + m_leftNode->toString() + " + " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeAdd::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeAdd(m_leftNode->copy(), m_rightNode->copy()));
}

std::unique_ptr<AstNode> AstNodeAdd::simplify() const {
    const auto left  = m_leftNode->simplify();
    const auto right = m_rightNode->simplify();
    if (left->isNumeric() && right->isNumeric()) {
        return doBinaryOperation(left, right, std::plus<>(), std::plus<>());
    }
    AstNode* simplifiedNode = new AstNodeAdd(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

bool AstNodeAdd::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::ADD) {
        const auto& candidate = dynamic_cast<const AstNodeAdd&>(other);
        return (*m_leftNode == *candidate.m_leftNode && *m_rightNode == *candidate.m_rightNode) ||
               (*m_rightNode == *candidate.m_leftNode && *m_leftNode == *candidate.m_rightNode);
    }
    return false;
}
