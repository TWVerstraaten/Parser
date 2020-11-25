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

  protected:
    void sortNodes();
    void mergeNodes();
    void cleanUp(const std::function<Numeric(const Numeric&, const Numeric&)>& accumulator,
                 const std::function<bool(const AstNode*)>&                           removePredicate);
    void accumulateNumeric(const std::function<Numeric(const Numeric&, const Numeric&)>& f);

    void                         removeChild(const AstNode* nodeToRemove) final;
    void                         addNode(std::unique_ptr<AstNode> node) final;
    [[nodiscard]] bool           equals(const AstNode& other) const final;
    [[nodiscard]] size_t         childCount() const final;
    [[nodiscard]] const AstNode* childAt(size_t index) const final;

    std::vector<std::unique_ptr<AstNode>> m_nodes;
};

#endif // PARSER_ASTNODECOMMUTATIVE_H
