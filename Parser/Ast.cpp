//
// Created by pc on 19-11-20.
//

#include "Ast.h"

#include "Parser.h"

Ast::Ast(std::string string) : m_string(std::move(string)), m_rootNode(Parser::parse(m_string)) {
}

Ast Ast::simplify() const {
    Ast result;
    result.m_string   = m_string;
    result.m_rootNode = m_rootNode->simplify(AstNode::SIMPLIFY_RULES::NONE);
    return result;
}

std::string Ast::toString() const {
    return m_rootNode->toString();
}
