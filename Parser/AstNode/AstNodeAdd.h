//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODEADD_H
#define PARSER_ASTNODEADD_H

#include "AstNode.h"

#include <vector>

class AstNodeAdd : public AstNode {

  public:
    AstNodeAdd(std::unique_ptr<AstNode>&& left, std::unique_ptr<AstNode>&& right);

    void                                            addNode(AstNode* node) override;
    std::vector<std::unique_ptr<AstNode>>::iterator removeChild(const AstNode* nodeToRemove) override;

    [[nodiscard]] std::string              toString() const override;
    [[nodiscard]] std::unique_ptr<AstNode> copy() const override;
    [[nodiscard]] std::unique_ptr<AstNode> simplify() const override;
    [[nodiscard]] NODE_TYPE                type() const override;
    [[nodiscard]] size_t                   childCount() const override;
    [[nodiscard]] const AstNode*           childAt(size_t index) const override;

  protected:
    [[nodiscard]] bool equals(const AstNode& other) const override;

  private:
    AstNodeAdd() = default;
    [[nodiscard]] std::unique_ptr<AstNode> simplifiedCopy() const;
    void                                   mergeAddends();
    void                                   sortAddends();
    void                                   addNumbers();

    std::vector<std::unique_ptr<AstNode>> m_nodes;
};

#endif // PARSER_ASTNODEADD_H
