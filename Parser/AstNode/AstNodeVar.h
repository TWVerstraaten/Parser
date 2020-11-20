//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEVAR_H
#define PARSER_ASTNODEVAR_H

#include "AstNode.h"

class AstNodeVar : public AstNode {
  public:
    explicit AstNodeVar(const std::string& name);

    [[nodiscard]] std::string toString() const override;

  private:
    std::string m_varName;
};

#endif // PARSER_ASTNODEVAR_H
