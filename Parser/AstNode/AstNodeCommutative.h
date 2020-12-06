//
// Created by pc on 25-11-20.
//

#ifndef PARSER_ASTNODECOMMUTATIVE_H
#define PARSER_ASTNODECOMMUTATIVE_H

#include "../../Algorithm/Algorithm.h"
#include "../Numeric.h"
#include "AstNode.h"

#include <functional>
#include <vector>

class AstNodeCommutative;

#define COMMUTATIVE_C_CAST(node) dynamic_cast<const AstNodeCommutative*>(node)
#define COMMUTATIVE_CAST(node) (dynamic_cast<AstNodeCommutative*>(node))

class AstNodeCommutative : public AstNode {

  public:
    void   addNode(u_ptr_AstNode node);
    void   removeNode(const AstNode* nodeToRemove);
    void   removeAllCopiesOfNode(const AstNode* nodeToRemove);
    void   removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE type);
    size_t countCopies(const AstNode* nodeToCompare) const;

    static Decomposition decompose(const AstNodeCommutative* A, const AstNodeCommutative* B);

    [[nodiscard]] const AstNode*                            childAt(size_t index) const final;
    [[nodiscard]] std::pair<const AstNode*, const AstNode*> findViaTypeContainingCopy(NODE_TYPE type) const;

  protected:
    AstNodeCommutative(std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)>                    removePredicate);
    AstNodeCommutative(std::vector<u_ptr_AstNode>&&                           nodes,
                       std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)>                    removePredicate);

    bool sortNodes();
    bool mergeNodes();
    bool cleanUp();
    bool accumulateNumeric();

    AstNode& operator[](size_t index);

    [[nodiscard]] bool   equals(const AstNode& other) const final;
    [[nodiscard]] bool   compareEqualType(const AstNode* rhs) const final;
    [[nodiscard]] size_t childCount() const final;

    [[nodiscard]] std::vector<u_ptr_AstNode>::const_iterator mulBegin() const;
    [[nodiscard]] std::vector<u_ptr_AstNode>::const_iterator mulEnd() const;
    [[nodiscard]] std::vector<u_ptr_AstNode>::const_iterator addBegin() const;
    [[nodiscard]] std::vector<u_ptr_AstNode>::const_iterator addEnd() const;

  protected:
    friend class AstNodeAdd;
    friend class AstNode;

    void                                                         removeNullptrs();
    const std::function<Numeric(const Numeric&, const Numeric&)> m_accumulator;
    const std::function<bool(u_ptr_AstNode&)>                    m_removePredicate;
    std::vector<u_ptr_AstNode>                                   m_nodes;
};

#endif // PARSER_ASTNODECOMMUTATIVE_H
