//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEMUL_H
#define PARSER_ASTNODEMUL_H

#include "AstNode.h"

class AstNodeMul : public AstNode {
  public:
    AstNodeMul(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;

  private:
    std::unique_ptr<AstNode> m_leftNode;
    std::unique_ptr<AstNode> m_rightNode;
};

#endif // PARSER_ASTNODEMUL_H
