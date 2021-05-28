//
// Created by pc on 19-11-20.
//

#include "AstNodeMul.h"

#include "AstNodeAdd.h"
#include "AstNodeUnaryMinus.h"

#include <cassert>

namespace ast {
    AstNodeMul::AstNodeMul()
        : AstNodeCommutative([](const gen::Number& lhs, const gen::Number& rhs) { return lhs * rhs; },
                             [](const u_ptr_AstNode& node) { return node->isOne(); }) {
    }

    AstNodeMul::AstNodeMul(u_ptr_AstNode&& left, u_ptr_AstNode&& right)
        : AstNodeCommutative([](const gen::Number& lhs, const gen::Number& rhs) { return lhs * rhs; },
                             [](u_ptr_AstNode& node) { return node->isOne(); }) {
        m_nodes.emplace_back(std::move(left));
        m_nodes.emplace_back(std::move(right));

        mergeNodes();
    }

    AstNodeMul::AstNodeMul(std::vector<u_ptr_AstNode>&& nodes)
        : AstNodeCommutative(
              std::move(nodes), [](const gen::Number& lhs, const gen::Number& rhs) { return lhs * rhs; },
              [](const u_ptr_AstNode& node) { return node->isOne(); }) {
        if (m_nodes.empty()) {
            m_nodes.emplace_back(AstNode::makeOneNode());
        }
        mergeNodes();
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

    u_ptr_AstNode AstNodeMul::copy() const {
        auto copy = u_ptr_AstNode(new AstNodeMul{});
        for (const auto& it : m_nodes) {
            COMMUTATIVE_CAST(copy.get())->addNode(it->copy());
        }
        return copy;
    }

    [[maybe_unused]] static u_ptr_AstNode potentiallyNegative(u_ptr_AstNode&& node, bool makeNegative) {
        return makeNegative ? std::make_unique<AstNodeUnaryMinus>(std::move(node)) : std::move(node);
    }

    u_ptr_AstNode AstNodeMul::simplify() const {
        std::unique_ptr<AstNodeMul> simplifiedNode = simplifiedCopy();
        simplifiedNode->cleanUp();
        if (simplifiedNode->m_nodes.empty()) {
            return makeOneNode();
        }
        if (simplifiedNode->m_nodes.size() == 1) {
            return simplifiedNode->m_nodes.front()->simplify();
        }
        if (std::find_if(simplifiedNode->m_nodes.begin(), simplifiedNode->m_nodes.end(),
                         [](const u_ptr_AstNode& a) { return a->isZero(); }) != simplifiedNode->m_nodes.end()) {
            return makeZeroNode();
        }
        return simplifiedNode;
    }

    AstNode::NODE_TYPE AstNodeMul::type() const {
        return NODE_TYPE::MULTIPLY;
    }

    std::unique_ptr<AstNodeMul> AstNodeMul::simplifiedCopy() const {
        auto copy = std::unique_ptr<AstNodeMul>(new AstNodeMul{});
        copy->m_nodes.reserve(m_nodes.size());
        std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(copy->m_nodes),
                       [](const u_ptr_AstNode& node) { return node->simplify(); });
        return copy;
    }

    u_ptr_AstNode AstNodeMul::distributeMultiplication() const {
        const auto addIt =
            std::find_if(m_nodes.begin(), m_nodes.end(), [](const u_ptr_AstNode& node) { return node->type() == AstNode::NODE_TYPE::ADD; });
        if (addIt == m_nodes.end()) {
            return copy();
        }
        size_t addends = (*addIt)->childCount();

        auto copyOfCurrentNode = copy();
        assert(*this == *copyOfCurrentNode);
        COMMUTATIVE_CAST(copyOfCurrentNode.get())->removeNode((*addIt).get());

        auto* result = new AstNodeAdd{};
        for (size_t i = 0; i != addends; ++i) {
            result->m_nodes.emplace_back(
                dynamic_cast<AstNodeMul*>(((*addIt)->childAt(i)->copy() * copyOfCurrentNode->copy()).get())->distributeMultiplication());
        }

        return u_ptr_AstNode(result);
    }

    u_ptr_AstNode AstNodeMul::differentiate(const std::string& variable) const {
        assert(not m_nodes.empty());
        if (m_nodes.size() == 1) {
            return m_nodes.front()->differentiate(variable);
        }
        const auto                 f = m_nodes.front()->copy();
        std::vector<u_ptr_AstNode> gNodes;
        std::transform(std::next(m_nodes.begin()), m_nodes.end(), std::back_inserter(gNodes), [](const auto& a) { return a->copy(); });
        const auto g = u_ptr_AstNode(new AstNodeMul(std::move(gNodes)));
        return u_ptr_AstNode(new AstNodeAdd(u_ptr_AstNode(new AstNodeMul(f->differentiate(variable), g->copy())),
                                            u_ptr_AstNode(new AstNodeMul(g->differentiate(variable), f->copy())))

        );
    }

    gen::Number AstNodeMul::eval(const std::map<std::string, gen::Number>& arguments) const {
        auto res = gen::Number(1ll);
        for (const auto& node : m_nodes) {
            res = res * node->eval(arguments);
        }
        return res;
    }
} // namespace ast
