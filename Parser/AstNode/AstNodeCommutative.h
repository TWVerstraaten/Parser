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
#define COMMUTATIVE_CAST(node) dynamic_cast<AstNodeCommutative*>(node)

typedef std::vector<u_ptr_AstNode> u_ptr_vec;

class AstNodeCommutative : public AstNode {

  public:
    void addNode(u_ptr_AstNode node);
    void removeNode(const AstNode* nodeToRemove);
    void removeNodeAndNodeWithSameChild(AstNode::NODE_TYPE type);

    friend Decomposition decompose(const AstNodeCommutative* A, const AstNodeCommutative* B);

    [[nodiscard]] size_t         indexOfCopy(const AstNode* node) const;
    [[nodiscard]] const AstNode* findViaTypeAndChild(NODE_TYPE type, const AstNode* node) const;
    [[nodiscard]] const AstNode* childAt(size_t index) const final;
    [[nodiscard]] std::pair<const AstNode*, const AstNode*> findViaTypeContainingCopy(NODE_TYPE type) const;

  protected:
    AstNodeCommutative(std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)>                    removePredicate);
    AstNodeCommutative(std::vector<u_ptr_AstNode>&&                           nodes,
                       std::function<Numeric(const Numeric&, const Numeric&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)>                    removePredicate);

    void sortNodes();
    void mergeNodes();
    void cleanUp();
    void accumulateNumeric();

    AstNode& operator[](size_t index);

    [[nodiscard]] bool   equals(const AstNode& other) const final;
    [[nodiscard]] bool   compareEqualType(const AstNode* rhs) const final;
    [[nodiscard]] size_t childCount() const final;

  protected:
    friend class AstNodeAdd;
    const std::function<Numeric(const Numeric&, const Numeric&)> m_accumulator;
    const std::function<bool(u_ptr_AstNode&)>                    m_removePredicate;
    std::vector<u_ptr_AstNode>                                   m_nodes;
};

#endif // PARSER_ASTNODECOMMUTATIVE_H
