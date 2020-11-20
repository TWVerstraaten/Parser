//
// Created by pc on 19-11-20.
//

#include "AstNodeSubtract.h"

AstNodeSubtract::AstNodeSubtract(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right)
    : AstNode(NODE_TYPE::SUB), m_leftNode(std::move(left)), m_rightNode(std::move(right)) {
}

std::string AstNodeSubtract::toString() const {
    return "(" + m_leftNode->toString() + " - " + m_rightNode->toString() + ")";
}
