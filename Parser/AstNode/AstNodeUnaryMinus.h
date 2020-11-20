//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEUNARYMINUS_H
#define PARSER_ASTNODEUNARYMINUS_H

#include "AstNode.h"

class AstNodeUnaryMinus : public AstNode {
  public:
    explicit AstNodeUnaryMinus(AstNode* value);

    [[nodiscard]] std::string toString() const override;

  private:
    std::unique_ptr<AstNode> m_value;
};

#endif // PARSER_ASTNODEUNARYMINUS_H
