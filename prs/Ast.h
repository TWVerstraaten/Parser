//
// Created by pc on 08-06-21.
//

#ifndef PRS_AST_H
#define PRS_AST_H

#include "AstToken.h"
#include "Header.h"
#include "ParserInfo.h"

#include <memory>

class Ast {

    friend class UnrolledAst;

  public:
    explicit Ast(const std::string& string);

    [[nodiscard]] bool              success() const;
    [[nodiscard]] const ParserInfo& info() const;

    [[nodiscard]] std::set<CustomFunction> dependsOn() const;
    [[nodiscard]] std::set<std::string>    variablesUsed() const;
    [[nodiscard]] std::vector<std::string> declaredVariables() const;

  private:
    void                          checkAndSetHeader();
    [[nodiscard]] const AstToken& body() const;

    ParserInfo                m_info;
    bool                      m_headerWasSet = false;
    Header                    m_header;
    std::unique_ptr<AstToken> m_rootNode;
    std::unique_ptr<AstToken> m_simplifiedNode;
    std::set<CustomFunction>  m_dependsOn;
};

#endif // PRS_AST_H
