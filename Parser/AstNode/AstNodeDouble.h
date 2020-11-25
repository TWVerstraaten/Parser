//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEDOUBLE_H
#define PARSER_ASTNODEDOUBLE_H

#include "AstNodeNumeric.h"

class AstNodeDouble : public AstNodeNumeric {

  public:
    explicit AstNodeDouble(const std::string& string);
    explicit AstNodeDouble(double value);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] double                   value() const;
    [[nodiscard]] size_t                   childCount() const override;
    [[nodiscard]] const AstNode*           childAt(size_t index) const override;

  protected:
    [[nodiscard]] Numeric toNumeric() const override;
    [[nodiscard]] bool    equals(const AstNode& other) const override;

    double m_value;
};

#endif // PARSER_ASTNODEDOUBLE_H
