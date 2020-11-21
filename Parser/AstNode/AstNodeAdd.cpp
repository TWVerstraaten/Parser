//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"
#include "AstNodeMul.h"
#include "Pattern/Pattern.h"

#include <cassert>
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
    std::unique_ptr<AstNode> simplifiedNode =
        std::unique_ptr<AstNode>(new AstNodeAdd(m_leftNode->simplify(), m_rightNode->simplify()));
    const auto* node = dynamic_cast<const AstNodeAdd*>(simplifiedNode.get());

    if (node->m_leftNode->isNumeric() && node->m_rightNode->isNumeric()) {
        return doBinaryOperation(node->m_leftNode, m_rightNode, std::plus<>(), std::plus<>());
    }

    Pattern pattern = Pattern::oneChildIs(Pattern::PATTERN_TOKEN::ZERO, true);
    if (pattern.match(node)) {
        return pattern.node("A")->copy();
    }

    Pattern childrenEqualPattern = Pattern::childrenEqual();
    if (childrenEqualPattern.match(node)) {
        return std::unique_ptr<AstNode>(
            new AstNodeMul(std::unique_ptr<AstNode>(new AstNodeInteger(2)), childrenEqualPattern.node("A")->copy()));
    }

    return simplifiedNode;
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

size_t AstNodeAdd::childCount() const {
    return 2ul;
}

const AstNode* AstNodeAdd::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}
