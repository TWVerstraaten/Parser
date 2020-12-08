//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEMUL_H
#define PARSER_ASTNODEMUL_H

#include "AstNodeCommutative.h"

class AstNodeMul : public AstNodeCommutative {
  public:
    AstNodeMul(u_ptr_AstNode&& left, u_ptr_AstNode&& right);
    explicit AstNodeMul(std::vector<u_ptr_AstNode>&& nodes);

    [[nodiscard]] NODE_TYPE     type() const override;
    [[nodiscard]] std::string   toString() const override;
    [[nodiscard]] u_ptr_AstNode copy() const override;
    [[nodiscard]] u_ptr_AstNode simplify(SIMPLIFY_RULES simplifyRules) const override;

  private:
    friend class AstNodeAdd;

    AstNodeMul();
    bool                                      stripUnaryMinuses();
    bool                                      gatherDuplicates();
    [[nodiscard]] std::unique_ptr<AstNodeMul> simplifiedCopy(SIMPLIFY_RULES simplifyRules) const;
    [[nodiscard]] u_ptr_AstNode               distributeMultiplication() const;
};

#endif // PARSER_ASTNODEMUL_H
