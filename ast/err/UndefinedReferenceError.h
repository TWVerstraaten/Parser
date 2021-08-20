//
// Created by pc on 15-07-21.
//

#ifndef AST_ERR_UNDEFINEDREFERENCEERROR_H
#define AST_ERR_UNDEFINEDREFERENCEERROR_H

#include "../Declaration.h"

#include <set>
#include <string>

namespace ast::err {
    class UndefinedReferenceError {

      public:
        UndefinedReferenceError(size_t index, std::set<Declaration>&& undefinedDeclarations);

        [[nodiscard]] std::string toString() const;

      private:
        size_t                m_index;
        std::set<Declaration> m_undefinedDeclarations;
    };
} // namespace ast::err

#endif // AST_ERR_UNDEFINEDREFERENCEERROR_H
