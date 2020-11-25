//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeDouble.h"
#include "AstNodeInteger.h"
#include "AstNodeMul.h"
#include "AstNodeUnaryMinus.h"
#include "Pattern/Pattern.h"

#include <cassert>
#include <iostream>
#include <memory>

AstNodeAdd::AstNodeAdd(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right) : m_nodes() {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));

    mergeAddends();
    sortAddends();
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
    return std::unique_ptr<AstNode>(new AstNodeAdd(m_nodes.front()->copy(), m_nodes.back()->copy()));
}

std::unique_ptr<AstNode> AstNodeAdd::simplify() const {
    std::unique_ptr<AstNode> simplifiedNode = simplifiedCopy();
    auto*                    node           = dynamic_cast<AstNodeAdd*>(simplifiedNode.get());
    node->addNumbers();
    node->removeIf([](const AstNode* node) { return node->isZero(); });
    node->removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE::UNARY_MINUS);

    //    node->transformNodeAndNodeWithSameChild(AstNode::NODE_TYPE::MULTIPLY);
    if (node->childCount() == 1) {
        return std::move(node->m_nodes[0]);
    }
    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

bool AstNodeAdd::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::ADD) {
        const auto& candidate = dynamic_cast<const AstNodeAdd&>(other);
        return (*m_nodes.front() == *candidate.m_nodes.front() && *m_nodes.back() == *candidate.m_nodes.back()) ||
               (*m_nodes.back() == *candidate.m_nodes.front() && *m_nodes.front() == *candidate.m_nodes.back());
    }
    return false;
}

size_t AstNodeAdd::childCount() const {
    return m_nodes.size();
}

const AstNode* AstNodeAdd::childAt(size_t index) const {
    assert(index < childCount());
    return m_nodes.at(index).get();
}

void AstNodeAdd::mergeAddends() {
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        if ((*it)->type() == AstNode::NODE_TYPE::ADD) {
            auto* childNode = dynamic_cast<AstNodeAdd*>(it->release());
            m_nodes.erase(it);
            std::move(childNode->m_nodes.begin(), childNode->m_nodes.end(), std::back_inserter(m_nodes));
            mergeAddends();
            return;
        }
    }
}

std::unique_ptr<AstNode> AstNodeAdd::simplifiedCopy() const {
    auto* result = new AstNodeAdd{};
    for (const auto& it : m_nodes) {
        result->m_nodes.emplace_back(it->simplify());
    }
    result->sortAddends();
    return std::unique_ptr<AstNode>(result);
}

void AstNodeAdd::sortAddends() {
    std::sort(m_nodes.begin(), m_nodes.end(), AstNode::compare);
}

void AstNodeAdd::addNumbers() {
    if (m_nodes.size() < 2) {
        return;
    }
    auto it = std::next(m_nodes.begin());
    while (it != m_nodes.end() && (*it)->isNumeric()) {
        std::unique_ptr<AstNode> firstNumber{(it - 1)->release()};
        std::unique_ptr<AstNode> secondNumber{(it)->release()};
        it = m_nodes.erase(it - 1);
        if (secondNumber->type() == AstNode::NODE_TYPE::INTEGER) {
            *it = std::make_unique<AstNodeInteger>(integerValue(firstNumber.get()) + integerValue(secondNumber.get()));
        } else {
            *it = std::make_unique<AstNodeDouble>(doubleValue(firstNumber.get()) + doubleValue(secondNumber.get()));
        }
        ++it;
    }
}

std::vector<std::unique_ptr<AstNode>>::iterator AstNodeAdd::removeChild(const AstNode* nodeToRemove) {
    assert(std::find_if(m_nodes.begin(), m_nodes.end(), [&](const std::unique_ptr<AstNode>& node) {
               return node.get() == nodeToRemove;
           }) != m_nodes.end());
    return m_nodes.erase(std::find_if(m_nodes.begin(), m_nodes.end(), [&](const std::unique_ptr<AstNode>& node) {
        return node.get() == nodeToRemove;
    }));
}

void AstNodeAdd::addNode(AstNode* node) {
    m_nodes.emplace_back(node);
}
