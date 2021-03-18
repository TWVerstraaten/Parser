//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeMul.h"

#include <cassert>

AstNodeAdd::AstNodeAdd()
    : AstNodeCommutative([](const Number& lhs, const Number& rhs) { return lhs + rhs; },
                         [](const u_ptr_AstNode& node) { return node->isZero(); }) {
}

AstNodeAdd::AstNodeAdd(std::vector<u_ptr_AstNode>&& nodes)
    : AstNodeCommutative(
          std::move(nodes), [](const Number& lhs, const Number& rhs) { return lhs + rhs; },
          [](const u_ptr_AstNode& node) { return node->isZero(); }) {
    if (m_nodes.empty()) {
        m_nodes.emplace_back(AstNode::makeZeroNode());
    }
    mergeNodes();
}

AstNodeAdd::AstNodeAdd(u_ptr_AstNode&& left, u_ptr_AstNode&& right)
    : AstNodeCommutative([](const Number& lhs, const Number& rhs) { return lhs + rhs; },
                         [](u_ptr_AstNode& node) { return node->isZero(); }) {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));
    mergeNodes();
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

u_ptr_AstNode AstNodeAdd::copy() const {
    auto copy = u_ptr_AstNode(new AstNodeAdd{});
    COMMUTATIVE_CAST(copy.get())->m_nodes.reserve(m_nodes.size());
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(COMMUTATIVE_CAST(copy.get())->m_nodes),
                   [](const auto& node) { return node->copy(); });
    return copy;
}

u_ptr_AstNode AstNodeAdd::simplify(SIMPLIFY_RULES simplifyRules) const {
    std::unique_ptr<AstNodeAdd> simplifiedNode = simplifiedCopy(AstNode::SIMPLIFY_RULES::DISTRIBUTE_MULTIPLICATION);
    simplifiedNode->cleanUp();
    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

std::unique_ptr<AstNodeAdd> AstNodeAdd::simplifiedCopy(SIMPLIFY_RULES simplifyRules) const {
    auto copy = std::unique_ptr<AstNodeAdd>(new AstNodeAdd{});
    copy->m_nodes.reserve(m_nodes.size());
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(copy->m_nodes),
                   [simplifyRules](const u_ptr_AstNode& node) { return node->simplify(simplifyRules); });
    return copy;
}

bool AstNodeAdd::gatherDuplicates() {
    assert(std::is_sorted(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr));
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        const auto count = countCopies(it->get());
        if (count > 1ul) {
            auto copy = (*it)->copy();
            m_nodes.erase(it + 1, it + count);
            *it = AstNode::makeNumber(static_cast<long long>(count)) * std::move(*it);
            return true;
        }
    }
    return false;
}

bool AstNodeAdd::cancelTerms() {
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        const auto it2 = std::find_if(m_nodes.begin(), m_nodes.end(), [&](const u_ptr_AstNode& node) {
            return node->type() == AstNode::NODE_TYPE::UNARY_MINUS && (*node->childAt(0) == **it);
        });
        if (it2 != m_nodes.end()) {
            it->reset(nullptr);
            it2->reset(nullptr);
            removeNullPointers();
            return true;
        }
    }
    return false;
}
