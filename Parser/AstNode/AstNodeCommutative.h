//
// Created by pc on 25-11-20.
//

#ifndef PARSER_ASTNODECOMMUTATIVE_H
#define PARSER_ASTNODECOMMUTATIVE_H

#include "../Numeric.h"
#include "AstNode.h"

#include <memory>
#include <vector>

class AstNodeCommutative : public AstNode {

  public:
    void                                                    addNode(u_ptr_AstNode node);
    void                                                    removeChild(const AstNode* nodeToRemove);
    void                                                    removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE type);
    [[nodiscard]] std::pair<const AstNode*, const AstNode*> findViaTypeContainingCopy(NODE_TYPE type) const;

  protected:
    AstNodeCommutative(std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)>                    removePredicate);

    void sortNodes();
    void mergeNodes();
    void cleanUp();
    void accumulateNumeric();

    [[nodiscard]] bool           equals(const AstNode& other) const final;
    [[nodiscard]] bool           compareEqualType(const AstNode* rhs) const final;
    [[nodiscard]] size_t         childCount() const final;
    [[nodiscard]] const AstNode* childAt(size_t index) const final;

  protected:
    const std::function<Numeric(const Numeric&, const Numeric&)> m_accumulator;
    const std::function<bool(u_ptr_AstNode&)>                    m_removePredicate;
    std::vector<u_ptr_AstNode>                                   m_nodes;
};

#endif // PARSER_ASTNODECOMMUTATIVE_H
