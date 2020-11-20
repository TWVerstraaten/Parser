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
    std::unique_ptr<AstNode> m_rootNode;
    const std::string        m_string;
};

#endif // PARSER_AST_H
