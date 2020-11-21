//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEDIV_H
#define PARSER_ASTNODEDIV_H

#include "AstNode.h"

class AstNodeDiv : public AstNode {
  public:
    AstNodeDiv(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    const AstNode*                         childAt(size_t index) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  public:
    size_t childCount() const override;

  private:
    std::unique_ptr<AstNode> m_leftNode;
    std::unique_ptr<AstNode> m_rightNode;
};

#endif // PARSER_ASTNODEDIV_H
