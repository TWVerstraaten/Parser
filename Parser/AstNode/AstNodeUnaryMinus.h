//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEUNARYMINUS_H
#define PARSER_ASTNODEUNARYMINUS_H

#include "AstNode.h"

class AstNodeUnaryMinus : public AstNode {
  public:
    explicit AstNodeUnaryMinus(std::unique_ptr<AstNode>&& value);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] size_t                   childCount() const override;
    [[nodiscard]] const AstNode*           childAt(size_t index) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    std::unique_ptr<AstNode> m_value;
};

#endif // PARSER_ASTNODEUNARYMINUS_H
