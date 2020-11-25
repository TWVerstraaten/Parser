//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEINTEGER_H
#define PARSER_ASTNODEINTEGER_H

#include "AstNode.h"

class AstNodeInteger : public AstNode {
  public:
    explicit AstNodeInteger(const std::string& string);
    explicit AstNodeInteger(long long int val);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] long long                value() const;
    [[nodiscard]] size_t                                 childCount() const override;
    [[nodiscard]] const AstNode*                                       childAt(size_t index) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    long long m_value;
};

#endif // PARSER_ASTNODEINTEGER_H
