//
// Created by pc on 19-11-20.
//

#include "Ast.h"

#include "Parser.h"

#include <cassert>
#include <utility>

Ast::Ast(std::string string)
    : m_string(std::move(string)), m_rootNode(Parser::parse(m_string)), m_copy(m_rootNode->simplify()) {
}

std::string Ast::toString() const {
    return m_copy->toString();
}
