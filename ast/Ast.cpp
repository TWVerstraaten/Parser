//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "../gen/Overloaded.h"
#include "err/DefinitionError.h"
#include "err/ParserError.h"
#include "err/ParserMessage.h"
#include "err/ParserWarning.h"
#include "par/Parser.h"
#include "par/TokenTemplates.h"

#include <QDebug>
#include <cassert>
#include <variant>

namespace ast {

    Ast::Ast(const std::string& string) {
        if (const auto rootOptional = par::Parser::S_PARSE(string, m_info); rootOptional.has_value()) {
            m_rootNode = rootOptional.value();
        }
        if (success()) {
            checkAndSetHeader();
        } else {
            m_info.printAll();
        }
    }

    bool Ast::success() const {
        return (not std::holds_alternative<par::AstToken::Error>(m_rootNode.token())) && m_info.success();
    }

    const err::ParserInfo& Ast::info() const {
        return m_info;
    }

    err::ParserInfo& Ast::info() {
        return m_info;
    }

    std::set<par::CustomFunctionToken> Ast::functionDependencies() const {
        return m_rootNode.getCustomFunctionDependencies();
    }

    std::set<std::string> Ast::variablesUsed() const {
        return body().variablesUsed();
    }

    std::vector<std::string> Ast::declaredVariables() const {
        assert(m_headerWasSet);
        switch (m_header.type()) {
            case Header::HEADER_TYPE::FULL_HEADER:
                return std::get<Header::FullHeader>(m_header.headerVariant()).m_variables;
            default:
                return {};
        }
    }

    void Ast::checkAndSetHeader() {
        if (not par::TokenTemplates::S_TOKEN_EQUALS<par::AstToken::OPERATOR_TYPE>(m_rootNode.token(), par::AstToken::OPERATOR_TYPE::EQUALS)) {
            m_header = Header{};
        } else {
            buildNonEmptyHeader();
        }
        m_headerWasSet = true;
    }

    void Ast::buildNonEmptyHeader() {
        assert(m_rootNode.children().size() == 2);
        const auto& headerAst = m_rootNode.children().front();
        std::visit(Overloaded{[this](const std::string& str) { m_header = Header{str}; },
                              [&](const par::CustomFunctionToken& function) {
                                  checkArgumentsIfFullHeader();
                                  if (m_info.success()) {
                                      m_header = Header(function, headerAst);
                                  }
                              },
                              [&](const par::VectorToken& vectorToken) { m_header = Header(vectorToken, headerAst); },
                              [&](const auto& a) {
                                  m_info.add({err::ParserError::TYPE::HEADER_ERROR, headerAst.toStringFlat(), headerAst.range()});
                              }},
                   headerAst.token());
    }

    const par::AstToken& Ast::body() const {
        if (std::holds_alternative<par::AstToken::OPERATOR_TYPE>(m_rootNode.token()) &&
            std::get<par::AstToken::OPERATOR_TYPE>(m_rootNode.token()) == par::AstToken::OPERATOR_TYPE::EQUALS) {
            assert(m_rootNode.children().size() == 2);
            return m_rootNode.children().back();
        } else {
            return m_rootNode;
        }
    }

    const Header& Ast::header() const {
        return m_header;
    }

    Ast Ast::replaceVariable(const std::string& variable, const ast::par::AstToken& token) const {
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

    void Ast::replaceVariableInPlace(const std::string& variable, const ast::par::AstToken& token) {
        m_rootNode.replaceVariable(variable, token);
    }

    void Ast::replaceFunctionInPlace(const Ast& functionToken) {
        assert(functionToken.m_header.type() == Header::HEADER_TYPE::FULL_HEADER);
        assert(std::holds_alternative<Header::FullHeader>(functionToken.m_header.headerVariant()));
        Header::FullHeader fullHeader = std::get<Header::FullHeader>(functionToken.m_header.headerVariant());
        assert(functionToken.m_rootNode.children().size() == 2);
        assert(std::holds_alternative<ast::par::AstToken::OPERATOR_TYPE>(functionToken.m_rootNode.token()));
        assert(std::get<ast::par::AstToken::OPERATOR_TYPE>(functionToken.m_rootNode.token()) == ast::par::AstToken::OPERATOR_TYPE::EQUALS);
        m_rootNode.replaceFunction(fullHeader, functionToken.body());
    }

    bool Ast::hasCustomDependencies() const {
        return not m_rootNode.getCustomFunctionDependencies().empty();
    }

    std::string Ast::toStringFlat() const {
        return m_rootNode.toStringFlat();
    }

    par::CustomFunctionToken Ast::getCustomFunctionToken() const {
        assert(m_header.type() == Header::HEADER_TYPE::FULL_HEADER);
        const auto& fullHeader = std::get<Header::FullHeader>(m_header.headerVariant());
        return par::CustomFunctionToken{fullHeader.m_name, fullHeader.m_variables.size()};
    }

    std::set<std::string> Ast::constantDependencies() const {
        return std::visit(Overloaded{
                              [this](const Header::ConstantHeader&) { return body().getUndeclaredVariables({}); },
                              [this](const Header::FullHeader&) {
                                  const auto declared = std::get<Header::FullHeader>(m_header.headerVariant()).m_variables;
                                  return body().getUndeclaredVariables(std::set<std::string>(declared.begin(), declared.end()));
                              },
                              [this](const auto&) {
                                  return body().getUndeclaredVariables({"x", "y", "z"});
                              },

                          },
                          m_header.headerVariant());
    }

    void Ast::checkArgumentsIfFullHeader() {
        assert(std::holds_alternative<ast::par::CustomFunctionToken>(m_rootNode.children().front().token()));
        const auto& arguments = m_rootNode.children().front().children();
        if (arguments.empty()) {
            m_info.add({err::ParserError::TYPE::NO_ARGUMENTS, "", m_rootNode.children().front().range()});
        }
        checkIfArgumentsAreStrings();
        if (m_info.success()) {
            checkRepeatedArguments();
        }
        if (m_info.success()) {
            checkUnusedArguments();
        }
    }

    void Ast::checkIfArgumentsAreStrings() {
        assert(std::holds_alternative<ast::par::CustomFunctionToken>(m_rootNode.children().front().token()));
        const auto& arguments = m_rootNode.children().front().children();
        for (const auto& argument : arguments) {
            if (not std::holds_alternative<std::string>(argument.token())) {
                if (std::holds_alternative<ast::par::AstToken::Empty>(argument.token())) {
                    m_info.add({err::ParserError::TYPE::EMPTY_ARGUMENT, "", m_rootNode.children().front().range()});
                } else {
                    m_info.add({err::ParserError::TYPE::NOT_AN_ARGUMENT, argument.toStringFlat(), argument.range()});
                }
            }
        }
    }

    void Ast::checkRepeatedArguments() {
        assert(std::holds_alternative<ast::par::CustomFunctionToken>(m_rootNode.children().front().token()));
        const auto& arguments = m_rootNode.children().front().children();
        if (arguments.size() > 1) {
            for (auto it1 = arguments.begin(); std::next(it1) != arguments.end(); ++it1) {
                const auto& argument = std::get<std::string>(it1->token());
                if (auto it2 = std::find_if(std::next(it1), arguments.end(), [&](const auto& a) { return std::get<std::string>(a.token()) == argument; }); it2 != arguments.end()) {
                    m_info.add({err::ParserError::TYPE::REPEATED_ARGUMENT, argument, it2->range()});
                    m_info.add({err::ParserError::TYPE::REPEATED_ARGUMENT, argument, it1->range()});
                }
            }
        }
    }

    void Ast::checkUnusedArguments() {
        assert(std::holds_alternative<ast::par::CustomFunctionToken>(m_rootNode.children().front().token()));
        const auto& arguments     = m_rootNode.children().front().children();
        const auto  usedVariables = variablesUsed();
        for (const auto& el : arguments) {
            const auto& argument = std::get<std::string>(el.token());
            if (usedVariables.find(argument) == usedVariables.end()) {
                m_info.add({err::ParserMessage::UNUSED_ARGUMENT, argument, el.range()});
            }
        }
    }

    Header::HEADER_TYPE Ast::headerType() const {
        return m_header.type();
    }

    bool Ast::isEmpty() const {
        return std::holds_alternative<ast::par::AstToken::Empty>(m_rootNode.token());
    }

    std::string Ast::getDeclaredName() const {
        assert(m_header.type() == Header::HEADER_TYPE::FULL_HEADER || m_header.type() == Header::HEADER_TYPE::CONSTANT);
        if (m_header.type() == ast::Header::HEADER_TYPE::FULL_HEADER) {
            return getCustomFunctionToken().name();
        } else if (m_header.type() == ast::Header::HEADER_TYPE::CONSTANT) {
            return std::get<ast::Header::ConstantHeader>(m_header.headerVariant()).m_name;
        }
        assert(false);
        return "";
    }
} // namespace ast