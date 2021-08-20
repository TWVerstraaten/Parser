//
// Created by pc on 16-08-21.
//

#ifndef AST_DEPENDENCYMANAGER_H
#define AST_DEPENDENCYMANAGER_H

#include "Ast.h"
#include "Dependency.h"

#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace ast {

    class DependencyManager {

        friend class UnrollManager;

        struct Vertex {
            struct Anonymous {};
            typedef std::variant<Anonymous, par::ConstantToken, par::FunctionToken> Type;

            Type                 m_type;
            std::set<Dependency> m_dependencies;
            std::set<size_t>     m_outEdges;
            std::set<size_t>     m_redeclarations;
        };

      public:
        void                                  addVertex(size_t index, const Ast& ast);
        void                                  removeVertex(size_t index);
        void                                  checkStatus(size_t index) const;
        void                                  clearUpdated();
        [[nodiscard]] std::string             toString() const;
        [[nodiscard]] const std::set<size_t>& updated() const;
        [[nodiscard]] std::vector<size_t>     unrollOrder() const;
        [[nodiscard]] std::set<size_t>        unrollable() const;

      private:
        void                        addVertex(size_t index, const Vertex::Type& type, const std::set<Dependency>& dependencies);
        void                        updateAdjacencies();
        void                        addRedeclarations(const std::string& addedName, size_t addedIndex);
        void                        checkRemovedAdjacencies();
        void                        addNewAdjacencies();
        void                        propagateUpdated();
        void                        unrollOrder(size_t start, std::vector<size_t>& partialOrder) const;
        bool                        checkForUpdatedDependency(size_t index);
        [[nodiscard]] bool          hasCircularDependency(std::vector<size_t>& currentPath) const;
        [[nodiscard]] bool          hasCircularDependency(size_t index) const;
        [[nodiscard]] size_t        get(const Dependency& dependency);
        [[nodiscard]] size_t        get(const par::FunctionToken& functionToken);
        [[nodiscard]] size_t        get(const par::ConstantToken& constantToken);
        [[nodiscard]] const Vertex& vertexAt(size_t index) const;

        std::set<size_t>         m_updated;
        std::map<size_t, Vertex> m_vertices;
    };

} // namespace ast

#endif // AST_DEPENDENCYMANAGER_H
