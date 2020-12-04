//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeInteger.h"
#include "AstNodeMul.h"
#include "AstNodeSubtract.h"

#include <cassert>

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
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(COMMUTATIVE_CAST(copy.get())->m_nodes),
                   [](const auto& node) { return node->copy(); });
    return copy;
}

u_ptr_AstNode AstNodeAdd::simplify() const {
    u_ptr_AstNode simplifiedNode = simplifiedCopy();
    auto*         node           = dynamic_cast<AstNodeAdd*>(simplifiedNode.get());
    node->cleanUp();
    node->removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE::UNARY_MINUS);
    //    node->makeSingleUnaryMinus();

    if (node->childCount() == 0) {
        return u_ptr_AstNode(new AstNodeInteger(0));
    } else if (node->childCount() == 1) {
        return std::move(node->m_nodes[0]);
    }

    //    if (node->gatherFactorsAndMultiplies() || node->gatherTermsAndMultiplies() ||
    //    node->gatherOverLappingMultiplies()) {
    //        return node->simplify();
    //    }
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
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(COMMUTATIVE_CAST(result)->m_nodes),
                   [](const auto& node) { return node->simplify(); });
    result->cleanUp();
    return u_ptr_AstNode(result);
}

bool AstNodeAdd::gatherOverLappingMultiplies() {
    for (auto it1 = m_nodes.begin(); it1 + 1 != m_nodes.end(); ++it1) {
        for (auto it2 = it1 + 1; it2 != m_nodes.end(); ++it2) {
            auto commonFactorStruct = intersect(it1->get(), it2->get());
            if (commonFactorStruct.m_common != nullptr) {
                m_nodes.erase(it2);
                m_nodes.erase(it1);
                m_nodes.emplace_back(std::make_unique<AstNodeAdd>(std::move(commonFactorStruct.m_firstRemainder),
                                                                  std::move(commonFactorStruct.m_secondRemainder)) *
                                     std::move(commonFactorStruct.m_common));
                return true;
            }
        }
    }
    return false;
}

bool AstNodeAdd::gatherTermsAndMultiplies() {
    for (auto it = mulBegin(); it != mulEnd(); ++it) {
        assert(it->get()->type() == AstNode::NODE_TYPE::MULTIPLY);
        const auto* mulChild = COMMUTATIVE_CAST(it->get());
        for (auto it2 = mulChild->addBegin(); it2 != mulChild->addEnd(); ++it2) {
            auto decomposition = decompose(this, COMMUTATIVE_C_CAST(it2->get()));
            if (decomposition.m_aCapB.empty()) {
                continue;
            }
            auto* commChild = COMMUTATIVE_CAST(it->get());
            commChild->m_nodes.erase(it2);
            u_ptr_AstNode childCopy = std::make_unique<AstNodeMul>(std::move(commChild->m_nodes));
            m_nodes.erase(it);
            for (const auto& intersectionIt : decomposition.m_aCapB) {
                removeNode(intersectionIt.get());
            }
            m_nodes.emplace_back((std::move(childCopy) + std::make_unique<AstNodeInteger>(1)) *
                                 std::make_unique<AstNodeAdd>(std::move(decomposition.m_aCapB)));
            return true;
        }
    }
    return false;
}

bool AstNodeAdd::gatherFactorsAndMultiplies() {
    for (auto it = mulBegin(); it != mulEnd(); ++it) {
        assert(it->get()->type() == AstNode::NODE_TYPE::MULTIPLY);
        auto* mulChild      = COMMUTATIVE_CAST(it->get());
        auto  decomposition = decompose(this, mulChild);
        if (decomposition.m_aCapB.empty()) {
            continue;
        }

        m_nodes.erase(it);
        for (const auto& it2 : decomposition.m_aCapB) {
            removeNode(it2.get());
        }
        m_nodes.emplace_back(
            u_ptr_AstNode(std::make_unique<AstNodeInteger>(1) +
                          std::make_unique<AstNodeMul>(std::move(decomposition.m_bMinusA))->simplify()) *
            std::make_unique<AstNodeMul>(std::move(decomposition.m_aCapB)));
        return true;
    }
    return false;
}

bool AstNodeAdd::hasUnaryMinusAddend() const {
    return std::find_if(m_nodes.begin(), m_nodes.end(), [](const auto& node) {
               return node->type() == AstNode::NODE_TYPE::UNARY_MINUS;
           }) != m_nodes.end();
}

void AstNodeAdd::makeSingleUnaryMinus() {
    if (not hasUnaryMinusAddend()) {
        return;
    }
    std::vector<u_ptr_AstNode> minus;
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        if ((*it)->type() == AstNode::NODE_TYPE::UNARY_MINUS) {
            minus.emplace_back((*it)->stealNode(0));
            it = m_nodes.erase(it);
        }
    }
    m_nodes.emplace_back(-std::make_unique<AstNodeAdd>(std::move(minus))->simplify());
}
