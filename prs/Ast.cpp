//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "Parser.h"

#include <cassert>
#include <iostream>

Ast::Ast(const std::string& string) {
    if (const auto rootOptional = Parser::parse(string, m_info); rootOptional.has_value()) {
        m_rootNode = rootOptional.value();
    }
    if (success()) {
        checkAndSetHeader();
        m_dependsOn = m_rootNode.getCustomFunctionDependencies();
    } else {
        m_info.printAll();
    }
}

bool Ast::success() const {
    return (not std::holds_alternative<AstToken::Error>(m_rootNode.token())) && m_info.success();
}

const ParserInfo& Ast::info() const {
    return m_info;
}

std::set<CustomFunctionToken> Ast::dependsOn() const {
    return m_dependsOn;
}

std::set<std::string> Ast::variablesUsed() const {
    return m_rootNode.variablesUsed();
}

std::vector<std::string> Ast::declaredVariables() const {
    assert(m_headerWasSet);
    switch (m_header.type()) {
        case Header::HEADER_TYPE::NAMED_AND_VARIABLES_DECLARED:
            return std::get<Header::FullHeader>(m_header.headerVariant()).m_variables;
        default:
            return {};
    }
}

void Ast::checkAndSetHeader() {
    if (not TokenTemplates::tokenEquals<AstToken::OPERATOR_TYPE>(m_rootNode.token(), AstToken::OPERATOR_TYPE::EQUALS)) {
        m_header = Header{};
    } else {
        buildNonEmptyHeader();
    }
    m_headerWasSet = true;
}

void Ast::buildNonEmptyHeader() {
    assert(m_rootNode.children().size() == 2);
    const auto& headerAst = m_rootNode.children().front();
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
            return m_rootNode;
        default:
            assert(m_rootNode.children().size() == 2);
            return m_rootNode.children().back();
    }
}

const Header& Ast::header() const {
    return m_header;
}

Ast Ast::replaceVariable(const std::string& variable, const AstToken& token) const {
    Ast result = *this;
    result.m_rootNode.replaceVariable(variable, token);
    return result;
}

Ast Ast::replaceFunction(const Ast& functionToken) const {
    Ast result = *this;
    result.replaceFunctionInPlace(functionToken);
    return result;
}

std::string Ast::toStringAsTree() const {
    return m_header.toString() + "\n" + body().toStringAsTree();
}

void Ast::replaceVariableInPlace(const std::string& variable, const AstToken& token) {
    m_rootNode.replaceVariable(variable, token);
}

void Ast::replaceFunctionInPlace(const Ast& functionToken) {
    assert(functionToken.m_header.type() == Header::HEADER_TYPE::NAMED_AND_VARIABLES_DECLARED);
    assert(std::holds_alternative<Header::FullHeader>(functionToken.m_header.headerVariant()));
    Header::FullHeader fullHeader = std::get<Header::FullHeader>(functionToken.m_header.headerVariant());
    assert(functionToken.m_rootNode.children().size() == 2);
    assert(std::holds_alternative<AstToken::OPERATOR_TYPE>(functionToken.m_rootNode.token()));
    assert(std::get<AstToken::OPERATOR_TYPE>(functionToken.m_rootNode.token()) == AstToken::OPERATOR_TYPE::EQUALS);
    m_rootNode.replaceFunction(fullHeader, functionToken.body());
    m_dependsOn = m_rootNode.getCustomFunctionDependencies();
}

bool Ast::hasCustomDependencies() const {
    assert(m_dependsOn == m_rootNode.getCustomFunctionDependencies());
    return not m_dependsOn.empty();
}

std::string Ast::toStringFlat() const {
    return m_rootNode.toStringFlat();
}
