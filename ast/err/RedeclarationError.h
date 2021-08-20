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
        RedeclarationError(size_t index, std::map<size_t, Declaration>&& otherDeclarations);

        [[nodiscard]] std::string toString() const;

      private:
        size_t                        m_index;
        std::map<size_t, Declaration> m_otherDeclarations;
    };
} // namespace ast::err

#endif // AST_ERR_REDECLARATIONERROR_H
