//
// Created by pc on 09-07-21.
//

#ifndef AST_ASTMANAGER_H
#define AST_ASTMANAGER_H

#include "../gen/DependencyGraph.h"
#include "Ast.h"
#include "UnrollCompanion.h"

#include <functional>
#include <map>
#include <vector>

namespace ast {

    class AstManager {
      public:
        AstManager();

        void add(size_t index, Ast&& ast);

        [[nodiscard]] std::string                 toString() const;
        [[nodiscard]] const gen::DependencyGraph& dependencyGraph() const;

      private:
        void setUnrollStatuses();

        [[nodiscard]] std::vector<UnrollCompanion>&   fromHeaderType(Header::HEADER_TYPE type);
        [[nodiscard]] std::optional<UnrollCompanion*> fromIndex(size_t index);
        [[nodiscard]] std::optional<UnrollCompanion*> fromFunctionName(const std::string& functionName);
        [[nodiscard]] std::optional<UnrollCompanion*> fromFunctionToken(const par::CustomFunctionToken& functionToken);
        [[nodiscard]] std::optional<UnrollCompanion*> fromComparator(const std::function<bool(const UnrollCompanion&)>& comparator);

        std::map<Header::HEADER_TYPE, std::vector<UnrollCompanion>> m_astTokens;
        gen::DependencyGraph                                        m_dependencyGraph;
    };
} // namespace ast

#endif // AST_ASTMANAGER_H
