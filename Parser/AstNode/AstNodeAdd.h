//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEADD_H
#define PARSER_ASTNODEADD_H

#include "AstNodeCommutative.h"

class AstNodeAdd : public AstNodeCommutative {

  public:
    AstNodeAdd(u_ptr_AstNode&& left, u_ptr_AstNode&& right);
    explicit AstNodeAdd(std::vector<u_ptr_AstNode>&& nodes);

    [[nodiscard]] std::string   toString() const override;
    [[nodiscard]] u_ptr_AstNode copy() const override;
    [[nodiscard]] u_ptr_AstNode simplify() const override;
    [[nodiscard]] NODE_TYPE     type() const override;

  private:
    friend class AstNodeMul;

    AstNodeAdd();

    bool gatherOverLappingNodes();
    bool gatherDuplicates();
    bool cancelTerms();

    [[nodiscard]] u_ptr_AstNode simplifiedCopy() const;
};

#endif // PARSER_ASTNODEADD_H
