//
// Created by pc on 19-11-20.
//

#include "AstNodeSubtract.h"

#include <cassert>

AstNodeSubtract::AstNodeSubtract(u_ptr_AstNode&& left, u_ptr_AstNode&& right) : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeSubtract::toString() const {
    return "(" + m_leftNode->toString() + " - " + m_rightNode->toString() + ")";
}

u_ptr_AstNode AstNodeSubtract::copy() const {
    return u_ptr_AstNode(new AstNodeSubtract(m_leftNode->copy(), m_rightNode->copy()));
}

u_ptr_AstNode AstNodeSubtract::simplify() const {
    const auto left  = m_leftNode->simplify();
    const auto right = m_rightNode->simplify();
    if (left->type() == AstNode::NODE_TYPE::NUMBER && right->type() == AstNode::NODE_TYPE::NUMBER) {
        std::cout << "Implement\n";
        assert(false);
        //        return (NUMERIC_CAST(left.get()) - NUMERIC_CAST(right.get())).toNode();
    }

    AstNode* simplifiedNode = new AstNodeSubtract(m_leftNode->simplify(), m_rightNode->simplify());
    return u_ptr_AstNode(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeSubtract::type() const {
    return NODE_TYPE::SUBTRACT;
}

bool AstNodeSubtract::equals(const AstNode& other) const {
    return other.type() == AstNode::NODE_TYPE::SUBTRACT && *m_leftNode == *other.childAt(0) && *m_rightNode == *other.childAt(1);
}

size_t AstNodeSubtract::childCount() const {
    return 2;
}

const AstNode* AstNodeSubtract::childAt(size_t index) const {
    assert(index < childCount());
    return index == 0 ? m_leftNode.get() : m_rightNode.get();
}

bool AstNodeSubtract::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    if (*m_leftNode == *rhs->childAt(0)) {
        return compare(m_rightNode.get(), rhs->childAt(1));
    } else {
        return compare(m_leftNode.get(), rhs->childAt(0));
    }
}
u_ptr_AstNode AstNodeSubtract::differentiate(const std::string& variable) const {
    return m_leftNode->differentiate(variable) - m_rightNode->differentiate(variable);
}

std::set<std::string> AstNodeSubtract::collectVariables() const {
    auto result = m_rightNode->collectVariables();
    result.merge(m_leftNode->collectVariables());
    return result;
}

Number AstNodeSubtract::eval(const std::map<std::string, Number>& arguments) const {
    return m_leftNode->eval(arguments) - m_rightNode->eval(arguments);
}
