//
// Created by pc on 08-06-21.
//

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "AstToken.h"
#include "ParserInfo.h"

#include <memory>

class Ast {

  public:
    explicit Ast(const std::string& string);

    [[nodiscard]] bool              success() const;
    [[nodiscard]] const ParserInfo& info() const;

  private:
    ParserInfo                m_info;
    std::unique_ptr<AstToken> m_rootNode;
};

#endif // PARSER_AST_H
