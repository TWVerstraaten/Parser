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

    [[nodiscard]] std::string    toString() const override;
    [[nodiscard]] u_ptr_AstNode  copy() const override;
    [[nodiscard]] u_ptr_AstNode  simplify(SIMPLIFY_RULES simplifyRules) const override;
    [[nodiscard]] NODE_TYPE      type() const override;
    [[nodiscard]] size_t         childCount() const override;
    [[nodiscard]] const AstNode* childAt(size_t index) const override;
    [[nodiscard]] bool           compareEqualType(const AstNode* rhs) const override;

  protected:
    [[nodiscard]] Numeric toNumeric() const override;
    [[nodiscard]] bool    equals(const AstNode& other) const override;

    double m_value;
};

#endif // PARSER_ASTNODEDOUBLE_H
