//
// Created by pc on 09-07-21.
//

#ifndef AST_ASTMANAGER_H
#define AST_ASTMANAGER_H

#include "../gen/DependencyGraph.h"
#include "Header.h"

#include <functional>
#include <map>
#include <vector>

namespace ast {

    class Ast;
    class UnrollCompanion;

    class AstManager {
      public:
        AstManager();
        ~AstManager();

        void update(size_t index, const Ast& ast);
        void remove(size_t index);

        [[nodiscard]] std::string                 toString() const;
        [[nodiscard]] const gen::DependencyGraph& dependencyGraph() const;

      private:
        void setAllToUnchecked();
        void unrollAll();
        void checkRedeclarations();
        void checkUndefinedReferences();
        void unroll(UnrollCompanion& current, const std::vector<size_t>& history = {});

        [[nodiscard]] std::vector<std::unique_ptr<UnrollCompanion>>::iterator fromIndex(size_t index);
        [[nodiscard]] std::vector<std::unique_ptr<UnrollCompanion>>::iterator fromFunctionToken(const par::FunctionToken& functionToken);
        [[nodiscard]] std::vector<std::unique_ptr<UnrollCompanion>>::iterator fromConstantName(const std::string& constantName);

        std::vector<std::unique_ptr<UnrollCompanion>> m_unrollCompanions;
        gen::DependencyGraph                          m_dependencyGraph;
    };
} // namespace ast

#endif // AST_ASTMANAGER_H
