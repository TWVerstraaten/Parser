//
// Created by pc on 08-06-21.
//

#ifndef PRS_AST_H
#define PRS_AST_H

#include "AstToken.h"
#include "Header.h"
#include "ParserInfo.h"

class Ast {

    Ast() = default;

  public:
    explicit Ast(const std::string& string);

    void replaceVariableInPlace(const std::string& variable, const AstToken& token);
    void replaceFunctionInPlace(const Ast& functionToken);

    [[nodiscard]] bool                          success() const;
    [[nodiscard]] bool                          hasCustomDependencies() const;
    [[nodiscard]] const ParserInfo&             info() const;
    [[nodiscard]] const Header&                 header() const;
    [[nodiscard]] std::set<CustomFunctionToken> dependsOn() const;
    [[nodiscard]] std::set<std::string>         variablesUsed() const;
    [[nodiscard]] std::vector<std::string>      declaredVariables() const;
    [[nodiscard]] const AstToken&               body() const;
    [[nodiscard]] Ast                           replaceVariable(const std::string& variable, const AstToken& token) const;
    [[nodiscard]] Ast                           replaceFunction(const Ast& functionToken) const;
    [[nodiscard]] std::string                   toStringAsTree() const;
    [[nodiscard]] std::string                   toStringFlat() const;

  private:
    void checkAndSetHeader();
    void buildNonEmptyHeader();

    ParserInfo                    m_info;
    bool                          m_headerWasSet = false;
    Header                        m_header;
    AstToken                      m_rootNode;
    std::set<CustomFunctionToken> m_dependsOn;
};

#endif // PRS_AST_H
