//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "Parser.h"

#include <cassert>
#include <iostream>

Ast::Ast(const std::string& string) : m_rootNode(Parser::parse(string, m_info)) {
    std::cout << "Parsed\t" << string << '\n';
    if (success()) {
        checkAndSetHeader();
        m_simplifiedNode = std::make_unique<AstToken>(*m_rootNode);
        std::cout << AstToken::toStringAsTree(*m_simplifiedNode) << '\n';
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

std::set<CustomFunctionToken> Ast::dependsOn() const {
    return m_rootNode->dependsOn();
}

std::set<std::string> Ast::variablesUsed() const {
    return m_rootNode->variablesUsed();
}

std::vector<std::string> Ast::declaredVariables() const {
    assert(m_headerWasSet);
    switch (m_header.type()) {
        case Header::HEADER_TYPE::NAMED_AND_VARIABLES_DECLARED:
            return std::get<Header::FullHeader>(m_header.m_header).m_variables;
        default:
            return {};
    }
}

void Ast::checkAndSetHeader() {
    if (not TokenTemplates::tokenEquals<AstToken::OPERATOR_TYPE>(m_rootNode->token(), AstToken::OPERATOR_TYPE::EQUALS)) {
        m_header = Header{};
    } else {
        buildNonEmptyHeader();
    }
    m_headerWasSet = true;
}

void Ast::buildNonEmptyHeader() {
    static const std::set<std::string> reservedCoordinates{"x", "y", "z"};
    assert(m_rootNode->children().size() == 2);
    const auto& headerAst = m_rootNode->children().front();
    if (std::holds_alternative<std::string>(headerAst.token())) {
        assert(headerAst.children().empty());
        m_header = Header{std::get<std::string>(headerAst.token())};
    } else if (std::holds_alternative<CustomFunctionToken>(headerAst.token())) {
        m_header = Header(std::get<CustomFunctionToken>(headerAst.token()), headerAst);
    } else {
        assert(std::holds_alternative<VectorToken>(headerAst.token()));
        m_header = Header(std::get<VectorToken>(headerAst.token()), headerAst);
    }
}

const AstToken& Ast::body() const {
    switch (m_header.type()) {
        case Header::HEADER_TYPE::EMPTY:
            return *m_rootNode;
        default:
            assert(m_rootNode->children().size() == 2);
            return m_rootNode->children().back();
    }
    assert(false);
}
