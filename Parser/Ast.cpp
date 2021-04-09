//
// Created by pc on 19-11-20.
//

#include "Ast.h"

#include "Parser.h"

#include <utility>

Ast::Ast(std::string string) : m_string(std::move(string)), m_rootNode(Parser::parse(m_string)) {
    std::cout << "Ast:\t" << toString() << " " << m_string << "\n";
}

Ast Ast::simplify() const {
    Ast result;
    result.m_string   = m_string;
    result.m_rootNode = m_rootNode->simplify();
    return result;
}

Ast Ast::differentiate(const std::string& variable) const {
    Ast result;
    result.m_rootNode = m_rootNode->simplify()->differentiate(variable)->simplify();
    result.m_string   = result.m_rootNode->toString();
    return result;
}

std::string Ast::toString() const {
    return m_rootNode->toString();
}

std::set<std::string> Ast::variables() const {
    return m_rootNode->collectVariables();
}
