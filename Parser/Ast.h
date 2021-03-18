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

    [[nodiscard]] Ast simplify() const;

  private:
    Ast() = default;

    std::string                    m_string;
    std::unique_ptr<const AstNode> m_rootNode;
};

#endif // PARSER_AST_H
