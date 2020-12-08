//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEERROR_H
#define PARSER_ASTNODEERROR_H

#include "AstNode.h"

class AstNodeError : public AstNode {
  public:
    enum class ERROR_TYPE { DEFAULT, DIVISION_BY_ZERO };

    AstNodeError() = default;
    explicit AstNodeError(ERROR_TYPE errorType);
    [[nodiscard]] u_ptr_AstNode  copy() const override;
    [[nodiscard]] std::string    toString() const override;
    [[nodiscard]] u_ptr_AstNode  simplify(SIMPLIFY_RULES simplifyRules) const override;
    [[nodiscard]] NODE_TYPE      type() const override;
    [[nodiscard]] size_t         childCount() const override;
    [[nodiscard]] const AstNode* childAt(size_t index) const override;
    [[nodiscard]] bool           compareEqualType(const AstNode* rhs) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    ERROR_TYPE m_errorType = ERROR_TYPE::DEFAULT;
};

#endif // PARSER_ASTNODEERROR_H
