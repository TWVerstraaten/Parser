//
// Created by pc on 19-11-20.
//

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "AstNode/AstNode.h"

#include <vector>

class Ast {
  public:
    explicit Ast(std::string string);

    [[nodiscard]] std::string           toString() const;
    [[nodiscard]] Ast                   simplify() const;
    [[nodiscard]] Ast                   differentiate(const std::string& variable) const;
    [[nodiscard]] std::set<std::string> variables() const;
    [[nodiscard]] const std::string&    errorString() const;
    [[nodiscard]] bool                  parseSuccessful() const;

  private:
    Ast() = default;

    bool                           m_parseSuccessful = true;
    std::string                    m_string;
    std::string                    m_errorString;
    std::unique_ptr<const AstNode> m_rootNode;
};

#endif // PARSER_AST_H
