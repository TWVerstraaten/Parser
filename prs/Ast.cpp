//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "Parser.h"
#include "TokenTemplates.h"

#include <iostream>

Ast::Ast(const std::string& string) : m_rootNode(Parser::parse(string, m_info)) {
    std::cout << "Parsed\t" << string << '\n';
    if (success()) {
        m_simplifiedNode = std::make_unique<AstToken>(*m_rootNode);
        std::cout << AstToken::printTree(*m_simplifiedNode) << '\n';
        m_dependsOn = m_rootNode->dependsOn();
        for (const auto& el : m_dependsOn) {
            std::cout << el.name() << '\n';
        }
        determineHeaderType();
    }
    m_info.printAll();
}

bool Ast::success() const {
    return m_rootNode && m_info.success();
}

const ParserInfo& Ast::info() const {
    return m_info;
}

std::set<CustomFunction> Ast::dependsOn() const {
    return m_rootNode->dependsOn();
}

std::set<std::string> Ast::variablesUsed() const {
    return m_rootNode->variablesUsed();
}

std::set<std::string> Ast::declaredVariables() const {
    return m_rootNode->declaredVariables();
}

void Ast::determineHeaderType() {
    if (TokenTemplates::tokenEquals<AstToken::OPERATOR_TYPE>(m_rootNode->m_token, AstToken::OPERATOR_TYPE::EQUALS)) {

    } else {
        m_headerType = HEADER_TYPE::EMPTY;
    };
}
