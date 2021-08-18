//
// Created by pc on 15-07-21.
//

#include "DeclarationError.h"

namespace ast::err {

    DeclarationError::DeclarationError(DeclarationErrorVariant&& rhs) : variant(std::move(rhs)) {
    }

    std::string DeclarationError::toString() const {
        return std::visit([](const auto& a) { return a.toString(); }, static_cast<const DeclarationErrorVariant&>(*this));
    }
} // namespace ast::err
