//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "Parser.h"

#include <iostream>

Ast::Ast(const std::string& string) : m_rootNode(Parser::parse(string, m_info)) {
    std::cout << "Parsed\t" << string << '\n';
    if (success()) {
        m_rootNode->printTree();
        m_dependsOn = m_rootNode->dependsOn();
        for (const auto& el : m_dependsOn) {
            std::cout << el.m_name << '\n';
        }
    }
    m_info.printAll();
}

bool Ast::success() const {
    return m_rootNode && m_info.success();
}

const ParserInfo& Ast::info() const {
    return m_info;
}
