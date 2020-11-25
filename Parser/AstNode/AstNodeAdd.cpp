//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeInteger.h"
#include "AstNodeUnaryMinus.h"

#include <iostream>
#include <memory>

AstNodeAdd::AstNodeAdd(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right) {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));

    cleanUp([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
            [](const AstNode* node) { return node->isZero(); });
}

std::string AstNodeAdd::toString() const {
    std::string result = "(";
    for (const auto& it : m_nodes) {
        result += it->toString() + " + ";
    }
    result = result.substr(0, result.length() - 3ul);
    result += ")";
    return result;
}

std::unique_ptr<AstNode> AstNodeAdd::copy() const {
    auto copy = std::unique_ptr<AstNode>(new AstNodeAdd{});
    for (const auto& it : m_nodes) {
        copy->addNode(it->copy());
    }
    return copy;
}

std::unique_ptr<AstNode> AstNodeAdd::simplify() const {
    std::unique_ptr<AstNode> simplifiedNode = simplifiedCopy();
    auto*                    node           = dynamic_cast<AstNodeAdd*>(simplifiedNode.get());
    node->cleanUp([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
                  [](const AstNode* node) { return node->isZero(); });
    node->removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE::UNARY_MINUS);

    //    node->transformNodeAndNodeWithSameChild(AstNode::NODE_TYPE::MULTIPLY);
    if (node->childCount() == 0) {
        return std::unique_ptr<AstNode>(new AstNodeInteger(0));
    } else if (node->childCount() == 0) {
        return std::move(node->m_nodes[0]);
    }
    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

std::unique_ptr<AstNode> AstNodeAdd::simplifiedCopy() const {
    auto* result = new AstNodeAdd{};
    for (const auto& it : m_nodes) {
        result->addNode(it->simplify());
    }
    result->sortNodes();
    return std::unique_ptr<AstNode>(result);
}

void AstNodeAdd::addNumbers() {
}
