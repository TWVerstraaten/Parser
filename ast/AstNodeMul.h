//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEMUL_H
#define PARSER_ASTNODEMUL_H

#include "AstNodeCommutative.h"

namespace ast {
    class AstNodeMul : public AstNodeCommutative {
      public:
        AstNodeMul(u_ptr_AstNode&& left, u_ptr_AstNode&& right);
        explicit AstNodeMul(std::vector<u_ptr_AstNode>&& nodes);

        [[nodiscard]] NODE_TYPE     type() const override;
        [[nodiscard]] std::string   toString() const override;
        [[nodiscard]] u_ptr_AstNode copy() const override;
        [[nodiscard]] u_ptr_AstNode simplify() const override;
        [[nodiscard]] u_ptr_AstNode differentiate(const std::string& variable) const override;
        [[nodiscard]] gen::Number   eval(const std::map<std::string, gen::Number>& arguments) const override;

      private:
        friend class AstNodeAdd;

        AstNodeMul();
        [[nodiscard]] std::unique_ptr<AstNodeMul> simplifiedCopy() const;
        [[nodiscard]] u_ptr_AstNode               distributeMultiplication() const;
    };
} // namespace originalAst

#endif // PARSER_ASTNODEMUL_H
