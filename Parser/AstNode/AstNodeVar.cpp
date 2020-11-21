//
// Created by pc on 19-11-20.
//

#include "AstNodeVar.h"

#include <cassert>
#include <utility>

AstNodeVar::AstNodeVar(std::string value) : m_variableName(std::move(value)) {
}

std::string AstNodeVar::toString() const {
    return m_variableName;
}

std::unique_ptr<AstNode> AstNodeVar::copy() const {
    return std::unique_ptr<AstNode>(new AstNodeVar(m_variableName));
}

std::unique_ptr<AstNode> AstNodeVar::simplify() const {
    return copy();
}

AstNode::NODE_TYPE AstNodeVar::type() const {
    return NODE_TYPE::VARIABLE;
}

bool AstNodeVar::equals(const AstNode& other) const {
    if (other.type() == AstNode::NODE_TYPE::VARIABLE) {
        return m_variableName == dynamic_cast<const AstNodeVar&>(other).m_variableName;
    }
    return false;
}

size_t AstNodeVar::childCount() const {
    return 0;
}

const AstNode* AstNodeVar::childAt(size_t index) const {
    assert(false);
}
