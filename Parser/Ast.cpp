//
// Created by pc on 19-11-20.
//

#include "Ast.h"

#include "Parser.h"

Ast::Ast(std::string string)
    : m_string(std::move(string)), m_rootNode(Parser::parse(m_string)), m_copy(m_rootNode->simplify()) {
}

std::string Ast::toString() const {
    return "Original:\t" + m_rootNode->toString() + "\nSimplified:\t " + m_copy->toString();
}
