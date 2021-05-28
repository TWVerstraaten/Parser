//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeMul.h"

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

u_ptr_AstNode AstNodeAdd::simplify() const {
    std::unique_ptr<AstNodeAdd> simplifiedNode = simplifiedCopy();
    simplifiedNode->cleanUp();
    if (simplifiedNode->m_nodes.empty()) {
        return makeZeroNode();
    }
    if (simplifiedNode->m_nodes.size() == 1) {
        return simplifiedNode->m_nodes.front()->simplify();
    }
    return simplifiedNode;
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}

std::unique_ptr<AstNodeAdd> AstNodeAdd::simplifiedCopy() const {
    auto copy = std::unique_ptr<AstNodeAdd>(new AstNodeAdd{});
    copy->m_nodes.reserve(m_nodes.size());
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(copy->m_nodes),
                   [](const u_ptr_AstNode& node) { return node->simplify(); });
    return copy;
}

u_ptr_AstNode AstNodeAdd::differentiate(const std::string& variable) const {
    std::vector<u_ptr_AstNode> differentiatedNodes;
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(differentiatedNodes),
                   [&](const auto& a) { return a->differentiate(variable); });
    return u_ptr_AstNode(new AstNodeAdd(std::move(differentiatedNodes)));
}

Number AstNodeAdd::eval(const std::map<std::string, Number>& arguments) const {
    Number res = 0ll;
    for (const auto& node : m_nodes) {
        res = res + node->eval(arguments);
    }
    return res;
}
