//
// Created by pc on 08-06-21.
//

#ifndef PRS_AST_H
#define PRS_AST_H

#include "AstToken.h"
#include "ParserInfo.h"

#include <memory>

class Ast {

  public:
    enum class HEADER_TYPE { EMPTY, ONLY_NAMED, NAMED_AND_VARIABLES_DECLARED };

    explicit Ast(const std::string& string);

    [[nodiscard]] bool              success() const;
    [[nodiscard]] const ParserInfo& info() const;

    [[nodiscard]] std::set<CustomFunction> dependsOn() const;
    [[nodiscard]] std::set<std::string>    variablesUsed() const;
    [[nodiscard]] std::set<std::string>    declaredVariables() const;

  private:
    void determineHeaderType();

    HEADER_TYPE               m_headerType;
    ParserInfo                m_info;
    std::unique_ptr<AstToken> m_rootNode;
    std::unique_ptr<AstToken> m_simplifiedNode;
    std::set<CustomFunction>  m_dependsOn;
};

#endif // PRS_AST_H
