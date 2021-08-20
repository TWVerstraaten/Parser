//
// Created by pc on 17-08-21.
//

#include "UnrollManager.h"

#include "../gen/defines.h"
#include "Ast.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace ast {

    void UnrollManager::add(size_t index, const Ast& ast) {
        if (auto it = m_asts.find(index); it != m_asts.end()) {
            remove(index);
        }
        m_asts.insert({index, ast});
        m_dependencyManager.addVertex(index, ast);
    }

    void UnrollManager::remove(size_t index) {
        if (auto it = m_asts.find(index); it != m_asts.end()) {
            const auto& ast = it->second;
            if (ast.isDeclaration()) {
                m_filledDependencies.erase(ast.getDeclarationToken());
            }
            m_asts.erase(it);
        }
        m_unrolledAsts.erase(index);
        m_dependencyManager.removeVertex(index);
    }

    void UnrollManager::unrollAll() {
        removeUpdatedFromUnrolled();
        const auto unrollOrder = m_dependencyManager.unrollOrder();
        for (const auto index : unrollOrder) {
            unroll(index);
        }
        m_dependencyManager.clearUpdated();
    }

    void UnrollManager::removeUpdatedFromUnrolled() {
        const auto& updated = m_dependencyManager.updated();
        for (auto it = m_unrolledAsts.begin(); it != m_unrolledAsts.end();) {
            if (updated.find(it->first) != updated.end()) {
                it = m_unrolledAsts.erase(it);
            } else {
                ++it;
            }
        }
        for (const auto& [index, ast] : m_asts) {
            if (ast.isDeclaration() && updated.find(index) != updated.end()) {
                m_filledDependencies.erase(ast.getDeclarationToken());
            }
        }
    }

    const UnrolledAst& UnrollManager::getUnrolledAt(size_t index) const {
        assert(m_unrolledAsts.find(index) != m_unrolledAsts.end());
        return m_unrolledAsts.at(index);
    }

    void UnrollManager::unroll(size_t index) {
        assert(m_asts.find(index) != m_asts.end());
        Ast        partiallyUnrolled = m_asts.at(index);
        const auto dependencies      = partiallyUnrolled.getDependencies();
        for (const auto& dependency : dependencies) {
            assert(m_filledDependencies.find(dependency) != m_filledDependencies.end());
            partiallyUnrolled.replaceDependencyInPlace(dependency, m_filledDependencies.at(dependency));
        }
        assert(partiallyUnrolled.getDependencies().empty());
        if (partiallyUnrolled.isDeclaration()) {
            m_filledDependencies.insert_or_assign(partiallyUnrolled.getDeclarationToken(), partiallyUnrolled);
        }
        m_unrolledAsts.insert({index, UnrolledAst{partiallyUnrolled}});
    }

    std::optional<err::DeclarationError> UnrollManager::checkForDeclarationError(size_t index) const {
        assert(m_asts.find(index) != m_asts.end());
        if (auto error = checkForReDeclarationError(index); error.has_value()) {
            return err::DeclarationError{error.value()};
        }
        if (auto error = checkForUndefinedReferenceError(index); error.has_value()) {
            return err::DeclarationError{error.value()};
        }
        if (auto error = checkForCircularDependencyError(index); error.has_value()) {
            return err::DeclarationError{error.value()};
        }
        return {};
    }

    std::optional<err::RedeclarationError> UnrollManager::checkForReDeclarationError(size_t index) const {
        const auto& vertex = m_dependencyManager.vertexAt(index);
        if (const auto& redeclarations = vertex.m_redeclarations; not redeclarations.empty()) {
            std::map<size_t, Declaration> r;
            std::transform(TT_IT(redeclarations), std::inserter(r, r.end()), [this](const size_t i) {
                return std::pair<size_t, Declaration>{i, m_asts.at(i).getDeclarationToken()};
            });
            return err::RedeclarationError{index, std::move(r)};
        } else {
            return {};
        }
    }

    std::optional<err::UndefinedReferenceError> UnrollManager::checkForUndefinedReferenceError(size_t index) const {
        const auto& vertex = m_dependencyManager.vertexAt(index);
        if (const auto& outEdges = vertex.m_outEdges; vertex.m_outEdges.size() != vertex.m_dependencies.size()) {
            assert(outEdges.size() < vertex.m_dependencies.size());
            auto undefinedReferences = vertex.m_dependencies;
            for (const auto& outEdge : outEdges) {
                assert(m_asts.at(outEdge).isDeclaration());
                assert(undefinedReferences.find(m_asts.at(outEdge).getDeclarationToken()) != undefinedReferences.end());
                undefinedReferences.erase(m_asts.at(outEdge).getDeclarationToken());
            }
            return err::UndefinedReferenceError{index, std::move(undefinedReferences)};
        } else {
            return {};
        }
    }

    std::optional<err::CircularDependencyError> UnrollManager::checkForCircularDependencyError(size_t index) const {
        if (std::vector<size_t> circularDependencies = {index}; m_dependencyManager.hasCircularDependency(circularDependencies)) {
            std::vector<std::pair<size_t, Declaration>> pathToCycle;
            std::transform(TT_IT(circularDependencies), std::inserter(pathToCycle, pathToCycle.end()), [this](const size_t i) {
                return std::make_pair(i, m_asts.at(i).getDeclarationToken());
            });
            return err::CircularDependencyError{index, std::move(pathToCycle)};
        } else {
            return {};
        }
    }

} // namespace ast