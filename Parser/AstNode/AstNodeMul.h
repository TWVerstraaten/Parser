//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEMUL_H
#define PARSER_ASTNODEMUL_H

#include "AstNode.h"
#include "AstNodeCommutative.h"

class AstNodeMul : public AstNodeCommutative {
  public:
    AstNodeMul(u_ptr_AstNode&& left, u_ptr_AstNode&& right);

    [[nodiscard]] std::string   toString() const override;
    [[nodiscard]] u_ptr_AstNode copy() const override;
    [[nodiscard]] u_ptr_AstNode simplify() const override;
    [[nodiscard]] NODE_TYPE     type() const override;

  private:
    AstNodeMul();
    [[nodiscard]] u_ptr_AstNode simplifiedCopy() const;
};

#endif // PARSER_ASTNODEMUL_H
