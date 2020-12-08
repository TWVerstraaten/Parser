//
// Created by pc on 08-12-20.
//

#ifndef PARSER_INTERSECTSTRUCT_H
#define PARSER_INTERSECTSTRUCT_H

#include <memory>

class AstNode;
typedef std::unique_ptr<AstNode> u_ptr_AstNode;

struct IntersectStruct {
    u_ptr_AstNode m_common{};
    u_ptr_AstNode m_firstRemainder{};
    u_ptr_AstNode m_secondRemainder{};

    u_ptr_AstNode&& firstOr(u_ptr_AstNode&& alternative) {
        return m_firstRemainder ? std::move(m_firstRemainder) : std::move(alternative);
    }
    u_ptr_AstNode&& secondOr(u_ptr_AstNode&& alternative) {
        return m_secondRemainder ? std::move(m_secondRemainder) : std::move(alternative);
    }
};

#endif // PARSER_INTERSECTSTRUCT_H
