//
// Created by pc on 19-11-20.
//

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "../gen/ErrorBase.h"
#include "AstNode.h"

#include <map>
#include <memory>
#include <vector>

namespace ast {
    class Ast : public gen::ErrorBase {
      public:
        explicit Ast(std::string string);

        [[nodiscard]] std::string           toString() const;
        [[nodiscard]] Ast                   simplify() const;
        [[nodiscard]] Ast                   differentiate(const std::string& variable) const;
        [[nodiscard]] std::set<std::string> variables() const;
        [[nodiscard]] gen::Number           eval(const std::map<std::string, gen::Number>& arguments) const;

      private:
        Ast() = default;

        std::string                    m_string;
        std::unique_ptr<const AstNode> m_rootNode;
    };
} // namespace ast
#endif // PARSER_AST_H
