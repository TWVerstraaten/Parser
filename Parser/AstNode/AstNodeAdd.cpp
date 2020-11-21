//
// Created by pc on 19-11-20.
//

#include "AstNodeAdd.h"

#include "AstNodeInteger.h"

#include <memory>

AstNodeAdd::AstNodeAdd(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeAdd::toString() const {
    return "(" + m_leftNode->toString() + " + " + m_rightNode->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeAdd::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeAdd(m_leftNode->copy(), m_rightNode->copy()));
}

std::unique_ptr<AstNode> AstNodeAdd::simplify() const {
//    auto left  = m_leftNode->simplify();
//    auto right = m_rightNode->simplify();
//
//    if (left->type() == NODE_TYPE::INTEGER && right->type() == NODE_TYPE::INTEGER) {
//        return std::unique_ptr<AstNode>(new AstNodeInteger(dynamic_cast<AstNodeInteger*>(left.get())->value() +
//                                                           dynamic_cast<AstNodeInteger*>(right.get())->value()));
//    }

    //    std::unique_ptr<AstNodeAdd> simplifiedNode =
    //        std::make_unique<AstNodeAdd>(m_leftNode->simplify(), m_rightNode->simplify());

    //    if (simplifiedNode->m_rightNode->type() == AstNode::NODE_TYPE::INTEGER &&
    //        simplifiedNode->m_leftNode->type() == AstNode::NODE_TYPE::INTEGER) {
    //        return std::unique_ptr<AstNode>(new
    //        AstNodeInteger(dynamic_cast<AstNodeInteger*>(m_rightNode.get())->value() +
    //                                                           dynamic_cast<AstNodeInteger*>(m_leftNode.get())->value()));
    //    }

    //    return std::move(simplifiedNode);

    AstNode* simplifiedNode = new AstNodeAdd(m_leftNode->simplify(), m_rightNode->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}

AstNode::NODE_TYPE AstNodeAdd::type() const {
    return NODE_TYPE::ADD;
}
