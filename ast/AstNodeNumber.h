//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODENUMBER_H
#define PARSER_ASTNODENUMBER_H

#include "../gen/Number.h"
#include "AstNode.h"

#include <unordered_set>

namespace ast {
    class AstNodeNumber : public AstNode {
      public:
        explicit AstNodeNumber(const std::string& string);
        explicit AstNodeNumber(long long int val);
        explicit AstNodeNumber(const gen::Number& number);

      public:
        [[nodiscard]] std::string                 toString() const override;
        [[nodiscard]] u_ptr_AstNode               copy() const override;
        [[nodiscard]] u_ptr_AstNode               simplify() const override;
        [[nodiscard]] NODE_TYPE                   type() const override;
        [[nodiscard]] size_t                      childCount() const override;
        [[nodiscard]] const AstNode*              childAt(size_t index) const override;
        [[nodiscard]] bool                        compareEqualType(const AstNode* rhs) const override;
        [[nodiscard]] const gen::Number&          value() const;
        [[nodiscard]] u_ptr_AstNode               differentiate(const std::string& variable) const override;
        [[nodiscard]] std::set<std::string>       usedVariables() const override;
        [[nodiscard]] gen::Number                 eval(const std::map<std::string, gen::Number>& arguments) const override;
        [[nodiscard]] std::set<fml::FunctionSignature> functionDependencies() const override;

      protected:
        [[nodiscard]] bool equals(const AstNode& other) const override;

      private:
        gen::Number m_value;
    };
} // namespace ast

#endif // PARSER_ASTNODENUMBER_H
