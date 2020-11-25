//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"
#include "AstNodePower.h"
#include "Pattern/Pattern.h"

#include <cassert>

AstNodeMul::AstNodeMul(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeMul::toString() const {
    return "(" + m_leftNode->toString() + " * " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeMul::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeMul(m_leftNode->copy(), m_rightNode->copy()));
}

std::unique_ptr<AstNode> AstNodeMul::simplify() const {
    std::unique_ptr<AstNode> simplifiedNode =
        std::unique_ptr<AstNode>(new AstNodeMul(m_leftNode->simplify(), m_rightNode->simplify()));
    const auto* node = dynamic_cast<const AstNodeMul*>(simplifiedNode.get());

    if (node->m_leftNode->isNumeric() && node->m_rightNode->isNumeric()) {
        return doBinaryOperation(node->m_leftNode, m_rightNode, std::multiplies<>(), std::multiplies<>());
    }

    Pattern zeroPattern = Pattern::oneOfTwoChildrenIs(Pattern::PATTERN_TOKEN::ZERO);
    if (zeroPattern.match(node)) {
        return std::unique_ptr<AstNode>(new AstNodeInteger(0));
    }
    Pattern onePattern = Pattern::oneOfTwoChildrenIs(Pattern::PATTERN_TOKEN::ONE);
    if (onePattern.match(node)) {
        return onePattern.node("A")->copy();
    }

    Pattern childrenEqualPattern = Pattern::childrenEqual();
    if (childrenEqualPattern.match(node)) {
        return std::unique_ptr<AstNode>(
            new AstNodePower(childrenEqualPattern.node("A")->copy(), std::unique_ptr<AstNode>(new AstNodeInteger(2))));
    }

    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeMul::type() const {
    return NODE_TYPE::MULTIPLY;
}

bool AstNodeMul::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::MULTIPLY) {
        const auto& candidate = dynamic_cast<const AstNodeMul&>(other);
        return (*m_leftNode == *candidate.m_leftNode && *m_rightNode == *candidate.m_rightNode) ||
               (*m_rightNode == *candidate.m_leftNode && *m_leftNode == *candidate.m_rightNode);
    }
    return false;
}

size_t AstNodeMul::childCount() const {
    return 2;
}

const AstNode* AstNodeMul::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}
