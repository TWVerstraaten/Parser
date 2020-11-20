//
// Created by pc on 20-11-20.
//

#ifndef PARSER_ASTNODEFUNCTION_H
#define PARSER_ASTNODEFUNCTION_H

#include "AstNode.h"

class AstNodeFunction : public AstNode {
  public:
    AstNodeFunction(const std::string& functionName, AstNode* argument);

    [[nodiscard]] std::string toString() const override;

  private:
    std::string              m_functionName;
    std::unique_ptr<AstNode> m_argument;
};

#endif // PARSER_ASTNODEFUNCTION_H
