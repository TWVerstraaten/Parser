//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEVAR_H
#define PARSER_ASTNODEVAR_H

#include "AstNode.h"

class AstNodeVar : public AstNode {
  public:
    explicit AstNodeVar(std::string name);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    std::string m_variableName;
};

#endif // PARSER_ASTNODEVAR_H
