//
// Created by pc on 15-07-21.
//

#ifndef AST_ERR_REDECLARATIONERROR_H
#define AST_ERR_REDECLARATIONERROR_H

#include "../Declaration.h"

#include <map>
#include <string>

namespace ast::err {
    class RedeclarationError {
      public:
        RedeclarationError(Declaration&& declaration, std::map<size_t, Declaration>&& otherDeclarations);

        [[nodiscard]] std::string toString() const;

      private:
        Declaration                   m_declaration;
        std::map<size_t, Declaration> m_otherDeclarations;
    };
} // namespace ast::err

#endif // AST_ERR_REDECLARATIONERROR_H
