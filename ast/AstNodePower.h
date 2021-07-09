//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEPOWER_H
#define PARSER_ASTNODEPOWER_H

#include "AstNode.h"

#include <unordered_set>

namespace ast {
    class AstNodePower : public AstNode {
      public:
        AstNodePower(u_ptr_AstNode&& base, u_ptr_AstNode&& exponent);

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
        u_ptr_AstNode m_base;
        u_ptr_AstNode m_exponent;
    };
} // namespace originalAst

#endif // PARSER_ASTNODEPOWER_H
