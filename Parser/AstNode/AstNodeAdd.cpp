//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include <cassert>

AstNodeAdd::AstNodeAdd()
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
                         [](const u_ptr_AstNode& node) { return node->isZero(); }) {
    cleanUp();
}

AstNodeAdd::AstNodeAdd(std::vector<u_ptr_AstNode>&& nodes)
    : AstNodeCommutative(
          std::move(nodes), [](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
          [](const u_ptr_AstNode& node) { return node->isZero(); }) {
    if (m_nodes.empty()) {
        m_nodes.emplace_back(AstNode::zero());
    }
    cleanUp();
}

AstNodeAdd::AstNodeAdd(u_ptr_AstNode&& left, u_ptr_AstNode&& right)
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
                         [](u_ptr_AstNode& node) { return node->isZero(); }) {
    m_nodes.emplace_back(std::move(left));
    m_nodes.emplace_back(std::move(right));
    cleanUp();
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

u_ptr_AstNode AstNodeAdd::simplify() const {
    u_ptr_AstNode simplifiedNode = simplifiedCopy();
    auto*         node           = dynamic_cast<AstNodeAdd*>(simplifiedNode.get());
    node->cleanUp();
    node->cancelTerms();
    if (node->childCount() == 0) {
        return AstNode::zero();
    } else if (node->childCount() == 1) {
        return std::move(node->m_nodes[0]);
    }

    if (node->gatherDuplicates() || node->gatherOverLappingNodes()) {
        return node->simplify();
    }

    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

u_ptr_AstNode AstNodeAdd::simplifiedCopy() const {
    auto copy = u_ptr_AstNode(new AstNodeAdd{});
    COMMUTATIVE_CAST(copy.get())->m_nodes.reserve(m_nodes.size());
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(COMMUTATIVE_CAST(copy.get())->m_nodes),
                   [](const auto& node) { return node->simplify(); });
    COMMUTATIVE_CAST(copy.get())->cleanUp();
    return copy;
}

bool AstNodeAdd::gatherOverLappingNodes() {
    for (auto it1 = m_nodes.begin(); it1 + 1 != m_nodes.end(); ++it1) {
        for (auto it2 = it1 + 1; it2 != m_nodes.end(); ++it2) {
            auto commonFactorStruct = intersect(it1->get(), it2->get());
            if (commonFactorStruct.m_common != nullptr) {
                m_nodes.erase(it2);
                m_nodes.erase(it1);
                m_nodes.emplace_back(
                    (commonFactorStruct.firstOr(AstNode::one()) + commonFactorStruct.secondOr(AstNode::one()))
                        ->simplify() *
                    std::move(commonFactorStruct.m_common));
                return true;
            }
        }
    }
    return false;
}

bool AstNodeAdd::gatherDuplicates() {
    assert(std::is_sorted(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr));
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        const auto count = countCopies(it->get());
        if (count > 1ul) {
            auto copy = (*it)->copy();
            m_nodes.erase(it + 1, it + count);
            *it = AstNode::makeInteger(static_cast<long long>(count)) * std::move(*it);
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
            removeNullptrs();
            return true;
        }
    }
    return false;
}
