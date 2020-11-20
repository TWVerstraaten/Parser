//
// Created by pc on 19-11-20.
//

#include "AstNodeUnaryMinus.h"

AstNodeUnaryMinus::AstNodeUnaryMinus(AstNode* value) : AstNode(AstNode::NODE_TYPE::UNARY_MINUS), m_value(value) {
}

std::string AstNodeUnaryMinus::toString() const {
    return "(-" + m_value->toString() + ")";
}
