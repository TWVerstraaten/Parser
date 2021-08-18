//
// Created by pc on 18-08-21.
//

#include "Declaration.h"

#include "../gen/Overloaded.h"

namespace ast {

    Declaration::Declaration(std::variant<par::ConstantToken, par::FunctionToken>&& variant) : std::variant<par::ConstantToken, par::FunctionToken>(variant) {
    }

    Declaration::Declaration(par::ConstantToken&& constantToken) : std::variant<par::ConstantToken, par::FunctionToken>(constantToken) {
    }

    Declaration::Declaration(par::FunctionToken&& functionToken) : std::variant<par::ConstantToken, par::FunctionToken>(functionToken) {
    }

    std::string Declaration::toString() const {
        return std::visit(Overloaded{[](const par::FunctionToken& function) { return function.toString(); }, [](const par::ConstantToken& constant) { return constant; }}, get());
    }
    std::variant<par::ConstantToken, par::FunctionToken>& Declaration::get() {
        return static_cast<std::variant<par::ConstantToken, par::FunctionToken>&>(*this);
    }
    const std::variant<par::ConstantToken, par::FunctionToken>& Declaration::get() const {
        return static_cast<const std::variant<par::ConstantToken, par::FunctionToken>&>(*this);
    }

} // namespace ast
