//
// Created by pc on 26-11-20.
//

#ifndef PARSER_ALGORITHM_H
#define PARSER_ALGORITHM_H

#include "../Parser/AstNode/AstNode.h"

#include <functional>
#include <vector>

struct Decomposition {

    std::vector<u_ptr_AstNode> m_aCapB;
    std::vector<u_ptr_AstNode> m_aMinusB;
    std::vector<u_ptr_AstNode> m_bMinusA;
};

namespace alg {
    Decomposition decomposeSorted(const std::vector<const AstNode*>& A, const std::vector<const AstNode*>& B,
                                  const std::function<u_ptr_AstNode(const AstNode*)>& transform);

} // namespace alg

#endif // PARSER_ALGORITHM_H
