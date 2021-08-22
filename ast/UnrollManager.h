//
// Created by pc on 17-08-21.
//

#ifndef AST_UNROLLMANAGER_H
#define AST_UNROLLMANAGER_H

#include "DependencyManager.h"
#include "UnrolledAst.h"
#include "err/DeclarationError.h"

#include <map>

namespace ast {
    class Ast;

    class UnrollManager {

      public:
        void                                               add(size_t index, const Ast& ast);
        void                                               remove(size_t index);
        void                                               unrollAll();
        void                                               removeUpdatedFromUnrolled();
        [[nodiscard]] const UnrolledAst&                   getUnrolledAt(size_t index) const;
        [[nodiscard]] std::optional<err::DeclarationError> checkForDeclarationError(size_t index) const;

      private:
        void unroll(size_t index);

        [[nodiscard]] std::optional<err::RedeclarationError>      checkForReDeclarationError(size_t index) const;
        [[nodiscard]] std::optional<err::UndefinedReferenceError> checkForUndefinedReferenceError(size_t index) const;
        [[nodiscard]] std::optional<err::CircularDependencyError> checkForCircularDependencyError(size_t index) const;

        std::map<size_t, Ast>         m_asts;
        std::map<size_t, UnrolledAst> m_unrolledAsts;
        std::map<Dependency, Ast>     m_filledDependencies;
        DependencyManager             m_dependencyManager;
    };

} // namespace ast

#endif // AST_UNROLLMANAGER_H
