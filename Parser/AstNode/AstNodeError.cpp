//
// Created by pc on 19-11-20.
//

#include "AstNodeError.h"

std::string AstNodeError::toString() const {
    return "Error";
}

std::unique_ptr<AstNode> AstNodeError::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeError{});
}

std::unique_ptr<AstNode> AstNodeError::simplify() const {
    return copy();
}

AstNode::NODE_TYPE AstNodeError::type() const {
    return NODE_TYPE::ERROR;
}
