//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEINTEGER_H
#define PARSER_ASTNODEINTEGER_H

#include "AstNodeNumeric.h"

class AstNodeInteger : public AstNodeNumeric {
  public:
    explicit AstNodeInteger(const std::string& string);
    explicit AstNodeInteger(long long int val);

  public:
    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] long long                value() const;
    [[nodiscard]] size_t                   childCount() const override;
    [[nodiscard]] const AstNode*           childAt(size_t index) const override;

  protected:
    [[nodiscard]] Numeric toNumeric() const override;
    [[nodiscard]] bool    equals(const AstNode& other) const override;

  private:
    long long m_value;
};

#endif // PARSER_ASTNODEINTEGER_H
