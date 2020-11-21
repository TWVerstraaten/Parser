//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

AstNodeUnaryMinus::AstNodeUnaryMinus(std::unique_ptr<AstNode>&& value) : m_value(std::move(value)) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}

std::unique_ptr<AstNode> AstNodeUnaryMinus::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeUnaryMinus(m_value->copy()));
}
std::unique_ptr<AstNode> AstNodeUnaryMinus::simplify() const {
    AstNode* simplifiedNode = new AstNodeUnaryMinus(m_value->simplify());
    return std::unique_ptr<AstNode>(simplifiedNode);
}
AstNode::NODE_TYPE AstNodeUnaryMinus::type() const {
    return NODE_TYPE::UNARY_MINUS;
}
