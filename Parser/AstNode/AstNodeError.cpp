//
// Created by pc on 19-11-20.
//

#include "AstNodeError.h"

#include <cassert>

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

bool AstNodeError::equals(const AstNode& other) const {
    return true;
}

size_t AstNodeError::childCount() const {
    return 0;
}

const AstNode* AstNodeError::childAt(size_t index) const {
    assert(false);
}
