//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"
#include "AstNodePower.h"

#include <cassert>

AstNodeMul::AstNodeMul(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right) {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));

    cleanUp([](const Numeric& lhs, const Numeric& rhs) { return lhs * rhs; },
            [](const AstNode* node) { return node->isOne(); });
}

std::string AstNodeMul::toString() const {
    std::string result = "(";
    for (const auto& it : m_nodes) {
        result += it->toString() + " * ";
    }
    result = result.substr(0, result.length() - 3ul);
    result += ")";
    return result;
}

std::unique_ptr<AstNode> AstNodeMul::copy() const {
    auto copy = std::unique_ptr<AstNode>(new AstNodeMul{});
    for (const auto& it : m_nodes) {
        copy->addNode(it->copy());
    }
    return copy;
}

std::unique_ptr<AstNode> AstNodeMul::simplify() const {
    std::unique_ptr<AstNode> simplifiedNode = simplifiedCopy();
    auto*                    node           = dynamic_cast<AstNodeMul*>(simplifiedNode.get());

    node->cleanUp([](const Numeric& lhs, const Numeric& rhs) { return lhs * rhs; },
                  [](const AstNode* node) { return node->isZero(); });

    if (node->childCount() == 0) {
        return std::unique_ptr<AstNode>(new AstNodeInteger(1));
    } else if (node->childCount() == 0) {
        return std::move(node->m_nodes[0]);
    }

    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeMul::type() const {
    return NODE_TYPE::MULTIPLY;
}

std::unique_ptr<AstNode> AstNodeMul::copyAllBut(const AstNode* nodeToSkip) const {
    auto copy = this->copy();
    copy->removeChild(nodeToSkip);

    return copy;
}

std::unique_ptr<AstNode> AstNodeMul::simplifiedCopy() const {
    auto* result = new AstNodeMul{};
    for (const auto& it : m_nodes) {
        result->addNode(it->simplify());
    }
    result->sortMultiplicants();
    return std::unique_ptr<AstNode>(result);
}

void AstNodeMul::sortMultiplicants() {
    std::sort(m_nodes.begin(), m_nodes.end(), AstNode::compare);
}
