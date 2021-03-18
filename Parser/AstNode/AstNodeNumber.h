//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODENUMBER_H
#define PARSER_ASTNODENUMBER_H

#include "AstNode.h"
#include "Number.h"

#include <variant>

class AstNodeNumber : public AstNode {
  public:
    explicit AstNodeNumber(const std::string& string);
    explicit AstNodeNumber(long long int val);
    explicit AstNodeNumber(const Number& number);

  public:
    [[nodiscard]] std::string    toString() const override;
    [[nodiscard]] u_ptr_AstNode  copy() const override;
    [[nodiscard]] u_ptr_AstNode  simplify(SIMPLIFY_RULES simplifyRules) const override;
    [[nodiscard]] NODE_TYPE      type() const override;
    [[nodiscard]] size_t         childCount() const override;
    [[nodiscard]] const AstNode* childAt(size_t index) const override;
    [[nodiscard]] bool           compareEqualType(const AstNode* rhs) const override;
    [[nodiscard]] const Number&  value() const;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    Number m_value;
};

#endif // PARSER_ASTNODENUMBER_H
