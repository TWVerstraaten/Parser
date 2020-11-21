//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEINTEGER_H
#define PARSER_ASTNODEINTEGER_H

#include "AstNode.h"

class AstNodeInteger : public AstNode {
  public:
    explicit AstNodeInteger(const std::string& string);
    explicit AstNodeInteger(size_t val);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] size_t value() const;

  private:
    size_t m_value;
};

#endif // PARSER_ASTNODEINTEGER_H
