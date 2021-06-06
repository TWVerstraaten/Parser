//
// Created by pc on 19-11-20.
//

#include "AstNodeVar.h"

#include <cassert>
#include <unordered_set>

namespace ast {
    AstNodeVar::AstNodeVar(std::string value) : m_variableName(std::move(value)) {
    }

    std::string AstNodeVar::toString() const {
        return m_variableName;
    }

    u_ptr_AstNode AstNodeVar::copy() const {
        return u_ptr_AstNode(new AstNodeVar(m_variableName));
    }

    u_ptr_AstNode AstNodeVar::simplify() const {
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

    bool AstNodeVar::compareEqualType(const AstNode* rhs) const {
        assert(rhs->type() == type());
        return m_variableName < dynamic_cast<const AstNodeVar*>(rhs)->m_variableName;
    }

    u_ptr_AstNode AstNodeVar::differentiate(const std::string& variable) const {
        return variable == m_variableName ? makeOneNode() : makeZeroNode();
    }

    std::set<std::string> AstNodeVar::usedVariables() const {
        return {m_variableName};
    }

    gen::Number AstNodeVar::eval(const std::map<std::string, gen::Number>& arguments) const {
        assert(arguments.find(m_variableName) != arguments.end());
        return arguments.at(m_variableName);
    }

    std::set<fml::FunctionSignature> AstNodeVar::functionDependencies() const {
        return {};
    }
} // namespace ast