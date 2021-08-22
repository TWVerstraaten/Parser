//
// Created by pc on 08-06-21.
//

#include "Ast.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"
#include "../gen/VariantTemplates.h"
#include "Dimension.h"
#include "par/Parser.h"
#include "par/TokenTemplates.h"

#include <cassert>

namespace ast {

    Ast::Ast(const std::string& string) {
        if (const auto rootOptional = par::Parser::S_PARSE(string, m_info); rootOptional.has_value()) {
            m_body = rootOptional.value();
        }
        if (success()) {
            if (not gen::S_VARIANT_EQUALS(m_body.token(), par::AstToken::OPERATOR_TYPE::EQUALS)) {
                m_header = Header{};
            } else {
                buildNonEmptyHeader();
            }
            m_headerWasSet = true;
        } else {
            //            m_info.printAll();
        }
    }

    bool Ast::success() const {
        return m_info.success() && not(std::holds_alternative<par::AstToken::Error>(m_body.token()) || std::holds_alternative<par::AstToken::Empty>(m_body.token()));
    }

    const err::ParserInfo& Ast::info() const {
        return m_info;
    }

    std::set<FunctionToken> Ast::getFunctionDependencies() const {
        return m_body.getFunctionDependencies();
    }

    std::set<std::string> Ast::variablesUsed() const {
        return body().variablesUsed();
    }

    std::vector<std::string> Ast::declaredVariables() const {
        assert(m_headerWasSet);
        switch (m_header.type()) {
            case Header::HEADER_TYPE::FULL_HEADER:
                return std::get<FullHeader>(m_header).m_variables;
            default:
                return {};
        }
    }

    void Ast::buildNonEmptyHeader() {
        assert(m_body.children().size() == 2);
        const auto& headerAst = m_body.children().front();
        std::visit(Overloaded{[this](const std::string& str) { m_header = Header{str}; },
                              [&](const FunctionToken& function) {
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
        m_body = m_body.children().back();
    }

    const par::AstToken& Ast::body() const {
        return m_body;
    }

    const Header& Ast::header() const {
        return m_header;
    }

    Ast Ast::replaceVariable(const std::string& variable, const ast::par::AstToken& token) const {
        Ast result = *this;
        result.m_body.replaceVariable(variable, token);
        return result;
    }

    Ast Ast::replaceFunction(const Ast& functionToken) const {
        Ast result = *this;
        result.replaceFunctionInPlace(functionToken);
        return result;
    }

    std::string Ast::toStringAsTree() const {
        return m_header.toString() + "\n" + m_body.toStringAsTree();
    }

    void Ast::replaceVariableInPlace(const std::string& variable, const ast::par::AstToken& token) {
        m_body.replaceVariable(variable, token);
    }

    void Ast::replaceFunctionInPlace(const Ast& function) {
        assert(function.m_header.type() == Header::HEADER_TYPE::FULL_HEADER);
        assert(std::holds_alternative<FullHeader>(function.m_header));
        m_body.replaceFunction(std::get<FullHeader>(function.m_header), function.body());
    }

    void Ast::replaceConstantInPlace(const Ast& constant) {
        assert(constant.m_header.type() == Header::HEADER_TYPE::CONSTANT);
        assert(std::holds_alternative<ConstantHeader>(constant.m_header));
        m_body.replaceConstant(std::get<ConstantHeader>(constant.m_header), constant.body());
    }

    std::string Ast::toStringFlat() const {
        if (headerType() == Header::HEADER_TYPE::EMPTY) {
            return m_body.toStringFlat();
        } else {
            return m_header.toString() + " = " + m_body.toStringFlat();
        }
    }

    FunctionToken Ast::getFunctionToken() const {
        assert(m_header.type() == Header::HEADER_TYPE::FULL_HEADER);
        const auto& fullHeader = std::get<FullHeader>(m_header);
        return FunctionToken{fullHeader.m_name, fullHeader.m_variables.size()};
    }

    ConstantToken Ast::getConstantToken() const {
        assert(m_header.type() == Header::HEADER_TYPE::CONSTANT);
        const auto& constantHeader = std::get<ConstantHeader>(m_header);
        assert(constantHeader == alg::str::TRIM(constantHeader));
        return constantHeader;
    }

    std::set<std::string> Ast::getConstantDependencies() const {
        return std::visit(Overloaded{
                              [this](const ConstantHeader&) { return body().getUndeclaredVariables({}); },
                              [this](const FullHeader&) {
                                  const auto declared = std::get<FullHeader>(m_header).m_variables;
                                  return m_body.getUndeclaredVariables(std::set<std::string>(declared.begin(), declared.end()));
                              },
                              [this](const auto&) {
                                  return m_body.getUndeclaredVariables({"x", "y", "z"});
                              },

                          },
                          static_cast<const HeaderVariant&>(m_header));
    }

    void Ast::checkArgumentsIfFullHeader() {
        assert(std::holds_alternative<ast::FunctionToken>(m_body.children().front().token()));
        const auto& arguments = m_body.children().front().children();
        if (arguments.empty()) {
            m_info.add({err::ParserError::TYPE::NO_ARGUMENTS, "", m_body.children().front().range()});
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
        assert(std::holds_alternative<ast::FunctionToken>(m_body.children().front().token()));
        const auto& arguments = m_body.children().front().children();
        for (const auto& argument : arguments) {
            if (not std::holds_alternative<std::string>(argument.token())) {
                if (std::holds_alternative<ast::par::AstToken::Empty>(argument.token())) {
                    m_info.add({err::ParserError::TYPE::EMPTY_ARGUMENT, "", m_body.children().front().range()});
                } else {
                    m_info.add({err::ParserError::TYPE::NOT_AN_ARGUMENT, argument.toStringFlat(), argument.range()});
                }
            }
        }
    }

    void Ast::checkRepeatedArguments() {
        assert(std::holds_alternative<ast::FunctionToken>(m_body.children().front().token()));
        const auto& arguments = m_body.children().front().children();
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
        assert(std::holds_alternative<ast::FunctionToken>(m_body.children().front().token()));
        const auto& arguments     = m_body.children().front().children();
        const auto  usedVariables = m_body.children().back().variablesUsed();
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
        return std::holds_alternative<ast::par::AstToken::Empty>(m_body.token());
    }

    std::string Ast::getDeclaredName() const {
        assert(m_header.type() == Header::HEADER_TYPE::FULL_HEADER || m_header.type() == Header::HEADER_TYPE::CONSTANT);
        if (m_header.type() == ast::Header::HEADER_TYPE::FULL_HEADER) {
            return getFunctionToken().name();
        } else if (m_header.type() == ast::Header::HEADER_TYPE::CONSTANT) {
            return std::get<ConstantHeader>(m_header);
        }
        assert(false);
        return "";
    }

    bool Ast::isDeclaration() const {
        return isFunction() || isConstant();
    }

    bool Ast::isFunction() const {
        assert(success());
        return m_header.type() == Header::HEADER_TYPE::FULL_HEADER;
    }

    bool Ast::isConstant() const {
        assert(success());
        return m_header.type() == Header::HEADER_TYPE::CONSTANT;
    }

    bool Ast::isAnonymous() const {
        return not isDeclaration();
    }

    std::set<Dependency> Ast::getDependencies() const {
        const auto           functionDependencies = getFunctionDependencies();
        const auto           constantDependencies = getConstantDependencies();
        std::set<Dependency> dependencies;
        for (const auto& function : functionDependencies) {
            dependencies.insert(Declaration{function});
        }
        for (const auto& constant : constantDependencies) {
            dependencies.insert(Declaration{constant});
        }
        return dependencies;
    }

    Declaration Ast::getDeclarationToken() const {
        assert(isDeclaration());
        if (isFunction()) {
            return Declaration{getFunctionToken()};
        } else {
            return Declaration{getConstantToken()};
        }
    }

    void Ast::replaceDependencyInPlace(const Dependency& dependency, const Ast& ast) {
        std::visit(Overloaded{[&](const FunctionToken& f) { replaceFunctionInPlace(ast); }, [&](const ConstantToken& c) { replaceConstantInPlace(ast); }}, dependency.get());
    }

    bool Ast::canUnroll() const {
        return getDependencies().empty();
    }

    bool Ast::dimensionsMatch() const {
        assert(canUnroll());
        return m_body.dimension() != DIMENSION_MISMATCH;
    }

    size_t Ast::dimension() const {
        return m_body.dimension();
    }
} // namespace ast