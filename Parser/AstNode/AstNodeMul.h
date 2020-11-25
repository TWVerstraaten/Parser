//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEMUL_H
#define PARSER_ASTNODEMUL_H

#include "AstNode.h"
#include "AstNodeCommutative.h"

class AstNodeMul : public AstNodeCommutative {
  public:
    AstNodeMul(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right);

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    std::unique_ptr<AstNode>               copyAllBut(const AstNode* nodeToSkip) const override;
    //    void                                   removeChild(const AstNode* nodeToRemove) override;

  private:
    AstNodeMul() = default;
    [[nodiscard]] std::unique_ptr<AstNode> simplifiedCopy() const;
    void                                   sortMultiplicants();
};

#endif // PARSER_ASTNODEMUL_H
