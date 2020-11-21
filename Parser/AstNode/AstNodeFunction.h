//
// Created by pc on 20-11-20.
//

#ifndef PARSER_ASTNODEFUNCTION_H
#define PARSER_ASTNODEFUNCTION_H

#include "AstNode.h"

class AstNodeFunction : public AstNode {
  public:
    AstNodeFunction(std::string functionName, std::unique_ptr<AstNode>&& argument);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;

  private:
    std::string              m_functionName;
    std::unique_ptr<AstNode> m_argument;
};

#endif // PARSER_ASTNODEFUNCTION_H
