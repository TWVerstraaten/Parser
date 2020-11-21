//
// Created by pc on 19-11-20.
//

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "AstNode/AstNode.h"

class Ast {
  public:
    explicit Ast(std::string string);

    [[nodiscard]] std::string toString() const;

  private:
    const std::string                    m_string;
    const std::unique_ptr<const AstNode> m_rootNode;
    std::unique_ptr<AstNode>             m_copy;
};

#endif // PARSER_AST_H
