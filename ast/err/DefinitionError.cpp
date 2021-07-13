//
// Created by pc on 10-07-21.
//

#include "DefinitionError.h"

#include <cassert>

namespace ast::err {

    DefinitionError::DefinitionError(DefinitionError::TYPE type, std::string message) : m_type(type), m_message(std::move(message)) {
    }

    std::string DefinitionError::toString() const {
        switch (m_type) {
            case TYPE::REDECLARATION:
                return "Redeclaration of " + m_message;
            case TYPE::UNDEFINED_FUNCTIONS:
                return "Undefined references: " + m_message;
        }
        assert(false);
        return "";
    }
} // namespace ast::err
