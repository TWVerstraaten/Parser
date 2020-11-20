//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEPOWER_H
#define PARSER_ASTNODEPOWER_H

#include "AstNode.h"

class AstNodePower : public AstNode {
  public:
    AstNodePower(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent);

    [[nodiscard]] std::string toString() const override;

  private:
    std::unique_ptr<AstNode> m_base;
    std::unique_ptr<AstNode> m_exponent;
};

#endif // PARSER_ASTNODEPOWER_H
