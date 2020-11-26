//
// Created by pc on 19-11-20.
//

#include "AstNodeError.h"

#include <cassert>

std::string AstNodeError::toString() const {
    return "Error";
}

u_ptr_AstNode AstNodeError::copy() const {
    return u_ptr_AstNode(new AstNodeError{});
}

u_ptr_AstNode AstNodeError::simplify() const {
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

bool AstNodeError::compareEqualType(const AstNode* rhs) const {
    assert(rhs->type() == type());
    return false;
}
