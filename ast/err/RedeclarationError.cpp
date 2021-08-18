//
// Created by pc on 15-07-21.
//

#include "RedeclarationError.h"

#include "../../alg/StringAlg.h"

namespace ast::err {

    RedeclarationError::RedeclarationError(Declaration&& declaration, std::map<size_t, Declaration>&& otherDeclarations)
        : m_declaration(declaration), m_otherDeclarations(otherDeclarations) {
    }

    std::string RedeclarationError::toString() const {
        return "Redeclaration of " + m_declaration.toString() + " as " + alg::str::CONCATENATE_STRINGS<std::pair<size_t, Declaration>>(m_otherDeclarations, [](const auto& pair) {
                   return "(" + std::to_string(pair.first) + ", " + pair.second.toString() + ")";
               });
    }

} // namespace ast::err