//
// Created by pc on 15-07-21.
//

#include "UndefinedReferenceError.h"

#include "../../alg/StringAlg.h"

namespace ast::err {

    UndefinedReferenceError::UndefinedReferenceError(size_t index, std::set<Declaration>&& undefinedDeclarations) : m_index(index), m_undefinedDeclarations(undefinedDeclarations) {
    }

    std::string UndefinedReferenceError::toString() const {
        return "Undefined references for " + std::to_string(m_index) + ": " + alg::str::CONCATENATE_STRINGS<Declaration>(m_undefinedDeclarations, &Declaration::toString);
    }
} // namespace ast::err