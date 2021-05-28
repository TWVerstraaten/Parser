//
// Created by pc on 19-11-20.
//

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "AstNode/AstNode.h"
#include "ErrorBase.h"

#include <map>
#include <vector>

class Ast : public ErrorBase {
  public:
    explicit Ast(std::string string);

    [[nodiscard]] std::string           toString() const;
    [[nodiscard]] Ast                   simplify() const;
    [[nodiscard]] Ast                   differentiate(const std::string& variable) const;
    [[nodiscard]] std::set<std::string> variables() const;
    [[nodiscard]] Number                eval(const std::map<std::string, Number> &arguments) const;

  private:
    Ast() = default;

    std::string                    m_string;
    std::unique_ptr<const AstNode> m_rootNode;
};

#endif // PARSER_AST_H
