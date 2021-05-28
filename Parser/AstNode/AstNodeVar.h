//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEVAR_H
#define PARSER_ASTNODEVAR_H

#include "AstNode.h"

class AstNodeVar : public AstNode {
  public:
    explicit AstNodeVar(std::string name);

    [[nodiscard]] std::string           toString() const override;
    [[nodiscard]] u_ptr_AstNode         copy() const override;
    [[nodiscard]] u_ptr_AstNode         simplify() const override;
    [[nodiscard]] NODE_TYPE             type() const override;
    [[nodiscard]] size_t                childCount() const override;
    [[nodiscard]] const AstNode*        childAt(size_t index) const override;
    [[nodiscard]] bool                  compareEqualType(const AstNode* rhs) const override;
    [[nodiscard]] u_ptr_AstNode         differentiate(const std::string& variable) const override;
    [[nodiscard]] std::set<std::string> collectVariables() const override;
    [[nodiscard]] Number                eval(const std::map<std::string, Number>& arguments) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    std::string m_variableName;
};

#endif // PARSER_ASTNODEVAR_H
