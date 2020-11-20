//
// Created by pc on 19-11-20.
//

#include "AstNode.h"

#include <utility>

AstNode::AstNode(AstNode::NODE_TYPE type) : m_type(type) {
}

AstNode::NODE_TYPE AstNode::type() const {
    return m_type;
}
