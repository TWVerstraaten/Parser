//
// Created by pc on 19-11-20.
//

#include "Ast.h"

#include "Parser.h"

#include <utility>

Ast::Ast(std::string string) : m_string(std::move(string)) {
    Parser parser{};
    m_rootNode = std::unique_ptr<AstNode>(parser.parse(m_string));
}

std::string Ast::toString() const {
    return m_rootNode->toString();
}
