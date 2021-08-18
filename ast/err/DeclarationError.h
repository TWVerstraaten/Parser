//
// Created by pc on 15-07-21.
//

#ifndef AST_ERR_DECLARATIONERROR_H
#define AST_ERR_DECLARATIONERROR_H

#include "CircularDependencyError.h"
#include "RedeclarationError.h"
#include "UndefinedReferenceError.h"

#include <string>
#include <variant>

namespace ast::err {

    typedef std::variant<UndefinedReferenceError, RedeclarationError, CircularDependencyError> DeclarationErrorVariant;

    class DeclarationError : private DeclarationErrorVariant {

      public:
        DeclarationError(DeclarationErrorVariant&& rhs);

        [[nodiscard]] std::string toString() const;
    };

} // namespace ast::err

#endif // AST_ERR_DECLARATIONERROR_H
