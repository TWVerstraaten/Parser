//
// Created by pc on 18-08-21.
//

#include "Declaration.h"

#include "../gen/Overloaded.h"

namespace ast {

    Declaration::Declaration(std::variant<ConstantToken, FunctionToken>&& variant) : std::variant<ConstantToken, FunctionToken>(variant) {
    }

    Declaration::Declaration(ConstantToken&& constantToken) : std::variant<ConstantToken, FunctionToken>(constantToken) {
    }

    Declaration::Declaration(FunctionToken&& functionToken) : std::variant<ConstantToken, FunctionToken>(functionToken) {
    }

    std::string Declaration::toString() const {
        return std::visit(
            Overloaded{[](const FunctionToken& function) { return function.toString(); }, [](const ConstantToken& constant) { return static_cast<std::string>(constant); }},
            get());
    }
    std::variant<ConstantToken, FunctionToken>& Declaration::get() {
        return static_cast<std::variant<ConstantToken, FunctionToken>&>(*this);
    }
    const std::variant<ConstantToken, FunctionToken>& Declaration::get() const {
        return static_cast<const std::variant<ConstantToken, FunctionToken>&>(*this);
    }

} // namespace ast
