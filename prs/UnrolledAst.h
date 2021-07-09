//
// Created by pc on 14-06-21.
//

#ifndef PRS_UNROLLEDAST_H
#define PRS_UNROLLEDAST_H

#include "Header.h"
#include "UnrolledAstToken.h"

class Ast;

class UnrolledAst {

  public:
    explicit UnrolledAst(const Ast& ast);

    [[nodiscard]] UnrolledAstToken setVariable(const std::string& variable, const gen::Number& number) const;
    [[nodiscard]] UnrolledAstToken setVariables(const std::map<std::string, gen::Number>& variableMap) const;

    [[nodiscard]] std::string toString() const;

  private:
    Header           m_header;
    UnrolledAstToken m_token;
};

#endif // PRS_UNROLLEDAST_H
