//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEDIV_H
#define PARSER_ASTNODEDIV_H

#include "AstNode.h"

class AstNodeDiv : public AstNode {
  public:
    AstNodeDiv(std::unique_ptr<AstNode>&& base, std::unique_ptr<AstNode>&& exponent);

    [[nodiscard]] std::string toString() const override;

  private:
    std::unique_ptr<AstNode> m_left;
    std::unique_ptr<AstNode> m_right;
};

#endif // PARSER_ASTNODEDIV_H
