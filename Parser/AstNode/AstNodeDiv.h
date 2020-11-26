//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEDIV_H
#define PARSER_ASTNODEDIV_H

#include "AstNode.h"

class AstNodeDiv : public AstNode {
  public:
    AstNodeDiv(u_ptr_AstNode&& base, u_ptr_AstNode&& exponent);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] u_ptr_AstNode copy() const override;
    [[nodiscard]] u_ptr_AstNode simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] const AstNode*                         childAt(size_t index) const override;
    bool                                                 compareEqualType(const AstNode* rhs) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  public:
    [[nodiscard]] size_t childCount() const override;

  private:
    u_ptr_AstNode m_leftNode;
    u_ptr_AstNode m_rightNode;
};

#endif // PARSER_ASTNODEDIV_H
