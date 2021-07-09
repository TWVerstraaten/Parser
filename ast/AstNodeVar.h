//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEVAR_H
#define PARSER_ASTNODEVAR_H

#include "AstNode.h"

#include <unordered_set>

namespace ast {
    class AstNodeVar : public AstNode {
      public:
        explicit AstNodeVar(std::string name);

        [[nodiscard]] std::string                 toString() const override;
        [[nodiscard]] u_ptr_AstNode               copy() const override;
        [[nodiscard]] u_ptr_AstNode               simplify() const override;
        [[nodiscard]] NODE_TYPE                   type() const override;
        [[nodiscard]] size_t                      childCount() const override;
        [[nodiscard]] const AstNode*              childAt(size_t index) const override;
        [[nodiscard]] bool                        compareEqualType(const AstNode* rhs) const override;
        [[nodiscard]] u_ptr_AstNode               differentiate(const std::string& variable) const override;
        [[nodiscard]] std::set<std::string>       usedVariables() const override;
        [[nodiscard]] gen::Number                 eval(const std::map<std::string, gen::Number>& arguments) const override;
        [[nodiscard]] std::set<fml::FunctionSignature> functionDependencies() const override;

      protected:
        [[nodiscard]] bool equals(const AstNode& other) const override;

      private:
        std::string m_variableName;
    };
} // namespace originalAst

#endif // PARSER_ASTNODEVAR_H
