//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeInteger.h"
#include "AstNodeMul.h"

AstNodeAdd::AstNodeAdd()
    : AstNodeCommutative([](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
                         [](const u_ptr_AstNode& node) { return node->isZero(); }) {
}

AstNodeAdd::AstNodeAdd(std::vector<u_ptr_AstNode>&& nodes)
    : AstNodeCommutative(
          std::move(nodes), [](const Numeric& lhs, const Numeric& rhs) { return lhs + rhs; },
          [](const u_ptr_AstNode& node) { return node->isZero(); }) {
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
    for (const auto& it : m_nodes) {
        COMMUTATIVE_CAST(copy.get())->addNode(it->copy());
    }
    return copy;
}

u_ptr_AstNode AstNodeAdd::simplify() const {
    u_ptr_AstNode simplifiedNode = simplifiedCopy();
    auto*         node           = dynamic_cast<AstNodeAdd*>(simplifiedNode.get());
    node->cleanUp();
    node->removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE::UNARY_MINUS);

    if (node->childCount() == 0) {
        return u_ptr_AstNode(new AstNodeInteger(0));
    } else if (node->childCount() == 1) {
        return std::move(node->m_nodes[0]);
    }

    if (node->gatherTermsAndMultiplies() || node->gatherOverLappingMultiplies()) {
        return node->simplify();
    }
    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

u_ptr_AstNode AstNodeAdd::simplifiedCopy() const {
    auto* result = new AstNodeAdd{};
    for (const auto& it : m_nodes) {
        result->addNode(it->simplify());
    }
    result->cleanUp();
    return u_ptr_AstNode(result);
}

bool AstNodeAdd::gatherOverLappingMultiplies() {
    for (size_t i = 0; i + 1 < childCount(); ++i) {
        for (size_t j = i + 1; j != childCount(); ++j) {
            if (childAt(i)->type() != AstNode::NODE_TYPE::MULTIPLY ||
                childAt(j)->type() != AstNode::NODE_TYPE::MULTIPLY) {
                continue;
            }
            auto decomposition = decompose(COMMUTATIVE_C_CAST(childAt(i)), COMMUTATIVE_C_CAST(childAt(j)));
            if (decomposition.m_aCapB.empty()) {
                continue;
            }
            removeNode(childAt(j));
            removeNode(childAt(i));
            auto left = std::make_unique<AstNodeMul>(std::move(decomposition.m_aMinusB)) +
                        std::make_unique<AstNodeMul>(std::move(decomposition.m_bMinusA));
            auto right = std::make_unique<AstNodeMul>(std::move(decomposition.m_aCapB));
            m_nodes.emplace_back(std::move(left) * std::move(right));
            return true;
        }
    }
    return false;
}

bool AstNodeAdd::gatherTermsAndMultiplies() {
    for (size_t i = 0; i < childCount(); ++i) {
        if (childAt(i)->type() != AstNode::NODE_TYPE::MULTIPLY) {
            continue;
        }
        for (size_t j = 0; j != childAt(i)->childCount(); ++j) {
            if (childAt(i)->childAt(j)->type() != AstNode::NODE_TYPE::ADD) {
                continue;
            }
            auto decomposition = decompose(this, COMMUTATIVE_C_CAST(childAt(i)->childAt(j)));
            if (decomposition.m_aCapB.empty()) {
                continue;
            }
            auto* commChild = COMMUTATIVE_CAST(&(*this)[i]);
            commChild->removeNode(commChild->childAt(j));
            u_ptr_AstNode childCopy = std::make_unique<AstNodeMul>(std::move(commChild->m_nodes));
            removeNode(childAt(i));
            for (const auto& it : decomposition.m_aCapB) {
                removeNode(it.get());
            }
            u_ptr_AstNode left  = std::move(childCopy) + std::make_unique<AstNodeInteger>(1);
            u_ptr_AstNode right = std::make_unique<AstNodeAdd>(std::move(decomposition.m_aCapB));
            m_nodes.emplace_back(std::move(left) * std::move(right));
            return true;
        }
    }
    return false;
}
