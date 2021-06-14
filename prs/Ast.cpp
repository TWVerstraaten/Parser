//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "Parser.h"
#include "TokenTemplates.h"

#include <cassert>
#include <iostream>

Ast::Ast(const std::string& string) : m_rootNode(Parser::parse(string, m_info)) {
    std::cout << "Parsed\t" << string << '\n';
    if (success()) {
        checkAndSetHeader();
        m_simplifiedNode = std::make_unique<AstToken>(*m_rootNode);
        std::cout << AstToken::printTree(*m_simplifiedNode) << '\n';
        m_dependsOn = m_rootNode->dependsOn();
        for (const auto& el : m_dependsOn) {
            std::cout << el.name() << '\n';
        }
        std::cout << m_header.toString() << '\n';
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

std::vector<std::string> Ast::declaredVariables() const {
    assert(m_headerWasSet);
    switch (m_header.type()) {
        case Header::HEADER_TYPE::NAMED_AND_VARIABLES_DECLARED:
            return std::get<Header::NamedAndVariablesHeader>(m_header.m_header).m_variables;
        default:
            return {};
    }
}

void Ast::checkAndSetHeader() {
    if (TokenTemplates::tokenEquals<AstToken::OPERATOR_TYPE>(m_rootNode->m_token, AstToken::OPERATOR_TYPE::EQUALS)) {
        assert(m_rootNode->m_children.size() == 2);
        const auto& headerAst = m_rootNode->m_children.front();
        if (std::holds_alternative<std::string>(headerAst.m_token)) {
            assert(headerAst.m_children.empty());
            const auto& name = std::get<std::string>(headerAst.m_token);
            m_header         = Header::OnlyNamedHeader{name};
        } else {
            assert(std::holds_alternative<CustomFunction>(headerAst.m_token));
            const auto&              function = std::get<CustomFunction>(headerAst.m_token);
            std::vector<std::string> variableNames;
            variableNames.reserve(function.argumentCount());
            for (const auto& child : headerAst.m_children) {
                assert(std::holds_alternative<std::string>(child.m_token));
                variableNames.emplace_back(std::get<std::string>(child.m_token));
            }
            m_header = Header::NamedAndVariablesHeader{function.name(), std::move(variableNames)};
        }
    } else {
        m_header = Header::EmptyHeader{};
    }
    m_headerWasSet = true;
}

const AstToken& Ast::body() const {
    switch (m_header.type()) {
        case Header::HEADER_TYPE::EMPTY:
            return *m_rootNode;
        default:
            assert(m_rootNode->m_children.size() == 2);
            return m_rootNode->m_children.back();
    }
    assert(false);
}
