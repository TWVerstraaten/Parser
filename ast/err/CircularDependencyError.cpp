//
// Created by pc on 18-08-21.
//

#include "CircularDependencyError.h"

#include "../../alg/StringAlg.h"

namespace ast::err {

    CircularDependencyError::CircularDependencyError(Declaration&& declaration, std::vector<std::pair<size_t, Declaration>>&& pathToCycle)
        : m_declaration(declaration), m_pathToCycle(pathToCycle) {
    }

    std::string CircularDependencyError::toString() const {
        return "Circular dependency for " + m_declaration.toString() + ": " + alg::str::CONCATENATE_STRINGS<std::pair<size_t, Declaration>>(m_pathToCycle, [](const auto& pair) {
                   return "(" + std::to_string(pair.first) + ", " + pair.second.toString() + ")";
               });
    }

} // namespace ast::err
