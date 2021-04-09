//
// Created by pc on 25-11-20.
//

#ifndef PARSER_ASTNODECOMMUTATIVE_H
#define PARSER_ASTNODECOMMUTATIVE_H

#include "../../Algorithm/Algorithm.h"
#include "AstNode.h"
#include "Number.h"

#include <functional>
#include <set>
#include <vector>

class AstNodeCommutative;

#define COMMUTATIVE_C_CAST(node) dynamic_cast<const AstNodeCommutative*>(node)
#define COMMUTATIVE_CAST(node) (dynamic_cast<AstNodeCommutative*>(node))

class AstNodeCommutative : public AstNode {

  public:
    void   addNode(u_ptr_AstNode node);
    void   removeNode(const AstNode* nodeToRemove);
    size_t countCopies(const AstNode* nodeToCompare) const;

    static Decomposition decompose(const AstNodeCommutative* A, const AstNodeCommutative* B);

    [[nodiscard]] const AstNode* childAt(size_t index) const final;

    [[nodiscard]] std::set<std::string> collectVariables() const override;

  protected:
    AstNodeCommutative(std::function<Number(const Number&, const Number&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)>                 removePredicate);
    AstNodeCommutative(std::vector<u_ptr_AstNode>&& nodes, std::function<Number(const Number&, const Number&)> accumulator,
                       std::function<bool(u_ptr_AstNode&)> removePredicate);

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
    friend class AstNodeUnaryMinus;

    void                                                      removeNullPointers();
    const std::function<Number(const Number&, const Number&)> m_accumulator;
    const std::function<bool(u_ptr_AstNode&)>                 m_removePredicate;
    std::vector<u_ptr_AstNode>                                m_nodes;
};

#endif // PARSER_ASTNODECOMMUTATIVE_H
