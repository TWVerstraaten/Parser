//
// Created by pc on 15-07-21.
//

#include "RedeclarationError.h"

#include "../../alg/StringAlg.h"

namespace ast::err {

    RedeclarationError::RedeclarationError(size_t index, std::map<size_t, Declaration>&& otherDeclarations) : m_index(index), m_otherDeclarations(otherDeclarations) {
    }

    std::string RedeclarationError::toString() const {
        return "Redeclaration of " + std::to_string(m_index) + " as " + alg::str::CONCATENATE_STRINGS<std::pair<size_t, Declaration>>(m_otherDeclarations, [](const auto& pair) {
                   return "(" + std::to_string(pair.first) + ", " + pair.second.toString() + ")";
               });
    }

} // namespace ast::err