//
// Created by pc on 27-11-20.
//

#include "Algorithm.h"

#include <algorithm>
#include <cassert>

Decomposition alg::decomposeSorted(const std::vector<const AstNode*>& A, const std::vector<const AstNode*>& B,
                                   std::function<u_ptr_AstNode(const AstNode*)> transform) {
    assert(std::is_sorted(A.begin(), A.end(), AstNode::compare));
    assert(std::is_sorted(B.begin(), B.end(), AstNode::compare));

    std::vector<u_ptr_AstNode> aCapB;
    std::vector<u_ptr_AstNode> aMinusB;
    std::vector<u_ptr_AstNode> bMinusA;
    auto                       bIt = B.cbegin();
    for (auto aIt = A.cbegin(); aIt != A.cend();) {
        if (bIt == B.cend()) {
            for (; aIt != A.cend(); ++aIt) {
                aMinusB.emplace_back(transform(*aIt));
            }
            break;
        }
        if (**aIt < **bIt) {
            aMinusB.emplace_back(transform(*aIt));
            ++aIt;
        } else if (**bIt == **aIt) {
            aCapB.emplace_back(transform(*bIt));
            ++aIt;
            ++bIt;
        } else {
            while (bIt != B.end() && **bIt < **aIt) {
                bMinusA.emplace_back(transform(*bIt));
                ++bIt;
            }
        }
    }
    for (; bIt != B.cend(); ++bIt) {
        bMinusA.emplace_back(transform(*bIt));
    }

    return {std::move(aCapB), std::move(aMinusB), std::move(bMinusA)};
}
