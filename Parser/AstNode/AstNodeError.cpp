//
// Created by pc on 19-11-20.
//

#include "AstNodeError.h"

AstNodeError::AstNodeError() : AstNode(NODE_TYPE::ERROR) {
}

std::string AstNodeError::toString() const {
    return "Error";
}
