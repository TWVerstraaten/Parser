//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEADD_H
#define PARSER_ASTNODEADD_H

#include "AstNode.h"
class AstNodeAdd : public AstNode {

  public:
    AstNodeAdd(AstNode* left, AstNode* right);

    [[nodiscard]] std::string toString() const override;

  private:
    std::unique_ptr<AstNode> m_leftNode;
    std::unique_ptr<AstNode> m_rightNode;
};

#endif // PARSER_ASTNODEADD_H
