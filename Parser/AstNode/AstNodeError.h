//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEERROR_H
#define PARSER_ASTNODEERROR_H

#include "AstNode.h"

class AstNodeError : public AstNode {
  public:
    AstNodeError() = default;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;
};

#endif // PARSER_ASTNODEERROR_H
