//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEMUL_H
#define PARSER_ASTNODEMUL_H

#include "AstNode.h"

class AstNodeMul : public AstNode {
  public:
    AstNodeMul(AstNode* base, AstNode* exponent);

    std::string toString() const override;

  private:
    std::unique_ptr<AstNode> m_left;
    std::unique_ptr<AstNode> m_right;
};

#endif // PARSER_ASTNODEMUL_H
