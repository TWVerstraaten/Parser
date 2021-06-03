//
// Created by pc on 19-11-20.
//

#include "AstNodeError.h"

#include <cassert>
#include <unordered_set>

namespace ast {
    AstNodeError::AstNodeError(AstNodeError::ERROR_TYPE errorType) : m_errorType(errorType) {
    }

    std::string AstNodeError::toString() const {
        switch (m_errorType) {
            case ERROR_TYPE::DEFAULT:
                return "Error";
            case ERROR_TYPE::DIVISION_BY_ZERO:
                return "Division by makeZeroNode!";
            default:
                assert(false);
        }
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

    u_ptr_AstNode AstNodeError::differentiate(const std::string& variable) const {
        return u_ptr_AstNode(new AstNodeError());
    }

    std::set<std::string> AstNodeError::usedVariables() const {
        return {};
    }
    gen::Number AstNodeError::eval(const std::map<std::string, gen::Number>& arguments) const {
        assert(false);
        return gen::Number(0ll);
    }

    std::set<FunctionSignature> AstNodeError::functionDependencies() const {
        return {};
    }
} // namespace ast