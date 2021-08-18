//
// Created by pc on 15-07-21.
//

#include "UndefinedReferenceError.h"

#include "../../alg/StringAlg.h"

namespace ast::err {

    UndefinedReferenceError::UndefinedReferenceError(Declaration&& declaration, std::set<Declaration>&& undefinedDeclarations)
        : m_declaration(declaration), m_undefinedDeclarations(undefinedDeclarations) {
    }

    std::string UndefinedReferenceError::toString() const {
        return "Undefined references for " + m_declaration.toString() + ": " + alg::str::CONCATENATE_STRINGS<Declaration>(m_undefinedDeclarations, &Declaration::toString);
    }
} // namespace ast::err