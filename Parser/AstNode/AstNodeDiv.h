//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEDIV_H
#define PARSER_ASTNODEDIV_H

#include "AstNode.h"

#include <set>

class AstNodeDiv : public AstNode {
  public:
    AstNodeDiv(u_ptr_AstNode&& numerator, u_ptr_AstNode&& denominator);

    [[nodiscard]] std::string           toString() const override;
    [[nodiscard]] u_ptr_AstNode         copy() const override;
    [[nodiscard]] u_ptr_AstNode         simplify() const override;
    [[nodiscard]] NODE_TYPE             type() const override;
    [[nodiscard]] const AstNode*        childAt(size_t index) const override;
    [[nodiscard]] bool                  compareEqualType(const AstNode* rhs) const override;
    [[nodiscard]] size_t                childCount() const override;
    [[nodiscard]] u_ptr_AstNode         differentiate(const std::string& variable) const override;
    [[nodiscard]] std::set<std::string> collectVariables() const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    u_ptr_AstNode m_numerator;
    u_ptr_AstNode m_denominator;
};

#endif // PARSER_ASTNODEDIV_H
