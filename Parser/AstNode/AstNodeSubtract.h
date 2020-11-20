//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODESUBTRACT_H
#define PARSER_ASTNODESUBTRACT_H

#include "AstNode.h"

class AstNodeSubtract : public AstNode {

  public:
    AstNodeSubtract(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right);

    [[nodiscard]] std::string toString() const override;

  private:
    std::unique_ptr<AstNode> m_leftNode;
    std::unique_ptr<AstNode> m_rightNode;
};

#endif // PARSER_ASTNODESUBTRACT_H
