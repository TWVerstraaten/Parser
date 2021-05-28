//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODENUMBER_H
#define PARSER_ASTNODENUMBER_H

#include "../Number.h"
#include "AstNode.h"

#include <variant>

class AstNodeNumber : public AstNode {
  public:
    explicit AstNodeNumber(const std::string& string);
    explicit AstNodeNumber(long long int val);
    explicit AstNodeNumber(const Number& number);

  public:
    [[nodiscard]] std::string           toString() const override;
    [[nodiscard]] u_ptr_AstNode         copy() const override;
    [[nodiscard]] u_ptr_AstNode         simplify() const override;
    [[nodiscard]] NODE_TYPE             type() const override;
    [[nodiscard]] size_t                childCount() const override;
    [[nodiscard]] const AstNode*        childAt(size_t index) const override;
    [[nodiscard]] bool                  compareEqualType(const AstNode* rhs) const override;
    [[nodiscard]] const Number&         value() const;
    [[nodiscard]] u_ptr_AstNode         differentiate(const std::string& variable) const override;
    [[nodiscard]] std::set<std::string> collectVariables() const override;
    [[nodiscard]] Number                eval(const std::map<std::string, Number>& arguments) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    Number m_value;
};

#endif // PARSER_ASTNODENUMBER_H
