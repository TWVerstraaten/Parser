//
// Created by pc on 20-11-20.
//

#ifndef PARSER_ASTNODEFUNCTION_H
#define PARSER_ASTNODEFUNCTION_H

#include "AstNode.h"
#include "ReservedFunction.h"

#include <vector>

namespace fml {
    class FormulaHeader;
}

namespace ast {
    class AstNodeFunction : public AstNode {

      public:
        AstNodeFunction(std::string functionName, std::vector<u_ptr_AstNode>&& arguments);

        [[nodiscard]] bool                        isReserved() const;
        [[nodiscard]] bool                        compareEqualType(const AstNode* rhs) const override;
        [[nodiscard]] size_t                      childCount() const override;
        [[nodiscard]] NODE_TYPE                   type() const override;
        [[nodiscard]] const AstNode*              childAt(size_t index) const override;
        [[nodiscard]] u_ptr_AstNode               copy() const override;
        [[nodiscard]] u_ptr_AstNode               simplify() const override;
        [[nodiscard]] u_ptr_AstNode               differentiate(const std::string& variable) const override;
        [[nodiscard]] gen::Number                 eval(const std::map<std::string, gen::Number>& arguments) const override;
        [[nodiscard]] std::string                 toString() const override;
        [[nodiscard]] std::set<std::string>       usedVariables() const override;
        [[nodiscard]] std::set<fml::FunctionSignature> functionDependencies() const override;

      protected:
        [[nodiscard]] bool equals(const AstNode& other) const override;

      private:
        std::unique_ptr<ReservedFunction> m_reservedFunction{nullptr};

        std::string                m_functionName;
        std::vector<u_ptr_AstNode> m_arguments;
    };
} // namespace originalAst

#endif // PARSER_ASTNODEFUNCTION_H
