//
// Created by pc on 19-11-20.
//

#ifndef PARSER_ASTNODE_H
#define PARSER_ASTNODE_H

#include <memory>
#include <string>
#include <utility>

class AstNode {
  public:
    enum class NODE_TYPE { VAR, NUM, UNARY_MINUS, ADD, SUB, MUL, DIV, POW, FUNCTION, ERROR };
    AstNode(NODE_TYPE type);

    virtual std::string     toString() const = 0;
    [[nodiscard]] NODE_TYPE type() const;

  protected:
    NODE_TYPE m_type;
};

#endif // PARSER_ASTNODE_H
