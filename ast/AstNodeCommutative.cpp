//
// Created by pc on 25-11-20.
//

#include "AstNodeCommutative.h"

#include "AstNodeNumber.h"

#include <cassert>
#include <unordered_set>

namespace ast {
    AstNodeCommutative::AstNodeCommutative(std::function<gen::Number(const gen::Number&, const gen::Number&)> accumulator,
                                           std::function<bool(u_ptr_AstNode&)>                                removePredicate)
        : m_accumulator(std::move(accumulator)), m_removePredicate(std::move(removePredicate)) {
    }

    AstNodeCommutative::AstNodeCommutative(std::vector<u_ptr_AstNode>&&                                       nodes,
                                           std::function<gen::Number(const gen::Number&, const gen::Number&)> accumulator,
                                           std::function<bool(u_ptr_AstNode&)>                                removePredicate)
        : m_accumulator(std::move(accumulator)), m_removePredicate(std::move(removePredicate)), m_nodes(std::move(nodes)) {
    }

    void AstNodeCommutative::removeNode(const AstNode* nodeToRemove) {
        assert(std::find_if(m_nodes.begin(), m_nodes.end(), [&](const u_ptr_AstNode& node) { return *node.get() == *nodeToRemove; }) !=
               m_nodes.end());
        m_nodes.erase(
            std::find_if(m_nodes.begin(), m_nodes.end(), [&](const u_ptr_AstNode& node) { return *node.get() == *nodeToRemove; }));
    }

    size_t AstNodeCommutative::childCount() const {
        return m_nodes.size();
    }

    const AstNode* AstNodeCommutative::childAt(size_t index) const {
        assert(index < childCount());
        return m_nodes.at(index).get();
    }

    void AstNodeCommutative::addNode(u_ptr_AstNode node) {
        m_nodes.emplace_back(std::move(node));
    }

    bool AstNodeCommutative::sortNodes() {
        if (std::is_sorted(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr)) {
            return false;
        }
        std::sort(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr);
        return true;
    }

    bool AstNodeCommutative::mergeNodes() {
        size_t requiredNumberOfNodes = m_nodes.size();
        for (const auto& node : m_nodes) {
            if (node->type() == type()) {
                requiredNumberOfNodes += node->childCount();
            }
        }
        m_nodes.reserve(requiredNumberOfNodes);
        bool somethingChanged = false;
        for (auto it = m_nodes.begin(); it != m_nodes.end();) {
            if ((*it)->type() != type()) {
                ++it;
            } else {
                somethingChanged = true;
                auto* childNode  = COMMUTATIVE_CAST(it->get());
                std::move(childNode->m_nodes.begin(), childNode->m_nodes.end(), std::back_inserter(m_nodes));
                it = m_nodes.erase(it);
            }
        }
        return somethingChanged;
    }

    bool AstNodeCommutative::equals(const AstNode& other) const {
        if (other.type() != type()) {
            return false;
        }
        if (other.childCount() != childCount()) {
            return false;
        }
        for (size_t i = 0; i != childCount(); ++i) {
            if (not(*m_nodes.at(i) == *other.childAt(i))) {
                return false;
            }
        }
        return true;
    }

    bool AstNodeCommutative::accumulateNumeric() {
        if (m_nodes.size() < 2) {
            return false;
        }
        assert(std::is_sorted(m_nodes.begin(), m_nodes.end(), AstNode::compare_u_ptr));
        auto it = std::next(m_nodes.begin());
        if (not(*it)->isNumeric()) {
            return false;
        }
        while (it != m_nodes.end() && (*it)->isNumeric()) {
            const gen::Number num1 = (*(it - 1))->getNumber();
            const gen::Number num2 = (*it)->getNumber();
            it                     = m_nodes.erase(it - 1);
            *it                    = u_ptr_AstNode(new AstNodeNumber(m_accumulator(num1, num2)));
            ++it;
        }
        return true;
    }

    bool AstNodeCommutative::cleanUp() {
        bool isChanged = false;
        isChanged |= mergeNodes();
        isChanged |= sortNodes();
        isChanged |= accumulateNumeric();
        if (std::find_if(m_nodes.begin(), m_nodes.end(), m_removePredicate) != m_nodes.end()) {
            isChanged = true;
            m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(), m_removePredicate), m_nodes.end());
        }
        return isChanged;
    }

    bool AstNodeCommutative::compareEqualType(const AstNode* rhs) const {
        assert(rhs->isCommutative());
        if (type() != rhs->type()) {
            return type() < rhs->type();
        }
        const auto& rightNodes = COMMUTATIVE_C_CAST(rhs)->m_nodes;
        return std::lexicographical_compare(m_nodes.begin(), m_nodes.end(), rightNodes.begin(), rightNodes.end(), AstNode::compare_u_ptr);
    }

    AstNode& AstNodeCommutative::operator[](size_t index) {
        return *m_nodes[index];
    }

    std::set<std::string> AstNodeCommutative::usedVariables() const {
        std::set<std::string> result;
        for (const auto& node : m_nodes) {
            result.merge(node->usedVariables());
        }
        return result;
    }

    std::set<fml::FunctionSignature> AstNodeCommutative::functionDependencies() const {
        std::set<fml::FunctionSignature> result;
        for (const auto& node : m_nodes) {
            result.merge(node->functionDependencies());
        }
        return result;
    }
} // namespace originalAst