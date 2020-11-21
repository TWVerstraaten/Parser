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
    enum class NODE_TYPE {
        VARIABLE,
        NUM,
        UNARY_MINUS,
        ADD,
        SUBTRACT,
        MUL,
        DIV,
        POWER,
        FUNCTION,
        ERROR,
        DOUBLE,
        INTEGER
    };
    explicit AstNode();

    [[nodiscard]] virtual std::string              toString() const = 0;
    [[nodiscard]] virtual std::unique_ptr<AstNode> copy() const     = 0;
    [[nodiscard]] virtual std::unique_ptr<AstNode> simplify() const = 0;
    [[nodiscard]] virtual NODE_TYPE                type() const     = 0;
};

#endif // PARSER_ASTNODE_H
