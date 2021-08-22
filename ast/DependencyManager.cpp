//
// Created by pc on 16-08-21.
//

#include "DependencyManager.h"

#include "../alg/StringAlg.h"
#include "../gen/Overloaded.h"
#include "../gen/VariantTemplates.h"
#include "../gen/defines.h"
#include "Ast.h"

#include <cassert>
#include <iostream>

namespace ast {

    void DependencyManager::addVertex(size_t index, const ast::Ast& ast) {
        assert(ast.success());
        addVertex(index,
                  ast.isFunction()   ? Vertex::Type(ast.getFunctionToken())
                  : ast.isConstant() ? Vertex::Type(ast.getConstantToken())
                                     : Vertex::Type(Vertex::Anonymous{}),
                  ast.getDependencies());
    }

    void DependencyManager::addVertex(size_t index, const DependencyManager::Vertex::Type& type, const std::set<Dependency>& dependencies) {
        if (m_vertices.find(index) != m_vertices.end()) {
            removeVertex(index);
        }
        m_vertices.insert({index, {type, dependencies}});
        m_updated.insert(index);
        if (not std::holds_alternative<Vertex::Anonymous>(type)) {
            std::string addedName = std::holds_alternative<ConstantToken>(type) ? std::get<ConstantToken>(type) : std::get<FunctionToken>(type).name();
            addRedeclarations(addedName, index);
        }
        updateAdjacencies();
    }

    void DependencyManager::removeVertex(size_t index) {
        m_vertices.erase(index);
        m_updated.erase(index);
        for (auto& [i, vertex] : m_vertices) {
            auto& outEdges = vertex.m_outEdges;
            if (outEdges.find(index) != outEdges.end()) {
                outEdges.erase(index);
                m_updated.insert(i);
            }
            auto& redeclarations = vertex.m_redeclarations;
            if (redeclarations.find(index) != redeclarations.end()) {
                redeclarations.erase(index);
                m_updated.insert(i);
            }
        }
        updateAdjacencies();
    }

    void DependencyManager::updateAdjacencies() {
        checkRemovedAdjacencies();
        addNewAdjacencies();
        propagateUpdated();
    }

    std::string DependencyManager::toString() const {
        std::stringstream ss;
        for (const auto& [index, vertex] : m_vertices) {
            ss << index << ": "
               << std::visit(Overloaded{[](const FunctionToken& f) { return f.toString(); },
                                        [](const ConstantToken& c) { return c; },
                                        [](const auto& a) { return std::string("Anon"); }},
                             vertex.m_type);
            ss << "\tUpdated: " << (m_updated.find(index) != m_updated.end());
            ss << "\tSatisfied Deps: ";
            for (const auto i : vertex.m_outEdges) {
                ss << i << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }

    void DependencyManager::addRedeclarations(const std::string& addedName, size_t addedIndex) {
        for (auto& [index, vertex] : m_vertices) {
            if (index == addedIndex || std::holds_alternative<Vertex::Anonymous>(vertex.m_type)) {
                continue;
            } else {
                const auto&       type  = vertex.m_type;
                const std::string other = std::holds_alternative<ConstantToken>(type) ? std::get<ConstantToken>(type) : std::get<FunctionToken>(type).name();
                if (other == addedName) {
                    vertex.m_redeclarations.insert(addedIndex);
                    m_vertices[addedIndex].m_redeclarations.insert(index);
                    m_updated.insert(index);
                }
            }
        }
    }

    void DependencyManager::checkRemovedAdjacencies() {
        for (auto& [index, vertex] : m_vertices) {
            auto& outEdges = vertex.m_outEdges;
            for (auto it = outEdges.begin(); it != outEdges.end();) {
                if (auto search = m_vertices.find(*it); search == m_vertices.end() || not search->second.m_redeclarations.empty()) {
                    it = outEdges.erase(it);
                    m_updated.insert(index);
                } else {
                    ++it;
                }
            }
        }
    }

    void DependencyManager::addNewAdjacencies() {
        for (auto& [index, vertex] : m_vertices) {
            auto& outEdges     = vertex.m_outEdges;
            auto& dependencies = vertex.m_dependencies;
            for (const auto& dependency : dependencies) {
                const auto i = get(dependency);
                if (i != std::numeric_limits<size_t>::max() && outEdges.find(i) == outEdges.end() && m_vertices[i].m_redeclarations.empty()) {
                    outEdges.insert(i);
                    m_updated.insert(index);
                }
            }
        }
    }

    void DependencyManager::propagateUpdated() {
        for (const auto& [index, vertex] : m_vertices) {
            if (not hasCircularDependency(index)) {
                checkForUpdatedDependency(index);
            } else {
                m_updated.insert(index);
            }
        }
    }

    bool DependencyManager::checkForUpdatedDependency(size_t index) {
        assert(not hasCircularDependency(index)); // Else will loop forever
        assert(m_vertices.find(index) != m_vertices.end());
        if (m_updated.find(index) != m_updated.end()) {
            return true;
        } else if (std::any_of(TT_IT(m_vertices[index].m_outEdges), [this](size_t outIndex) { return checkForUpdatedDependency(outIndex); })) {
            m_updated.insert(index);
            return true;
        }
        return false;
    }

    void DependencyManager::checkStatus(size_t index) const {
        assert(m_vertices.find(index) != m_vertices.end());
        const auto& vertex         = m_vertices.at(index);
        const auto& redeclarations = vertex.m_redeclarations;
        if (not redeclarations.empty()) {
            std::set<size_t> res = redeclarations;
            res.insert(index);
            std::cout << "Redeclarations:\t" << alg::str::CONCATENATE_STRINGS<size_t>(res, [](size_t s) { return std::to_string(s); }) << "\n";
        }

        //        const auto& outEdges = vertex.m_outEdges;
        //        for (const auto& dependency : vertex.m_dependencies) {
        //            if (auto it = outEdges.find(get(dependency)); it != outEdges.end()) {
        //                assert(m_vertices[*it].m_redeclarations.empty());
        //            } else {
        //                                std::cout << "Undefined function:\t" << dependency.toString() << "\n";
        //            }
        //        }
        std::vector<size_t> cycle = {0};
        if (hasCircularDependency(cycle)) {
            std::cout << "Circular dependency:\t" << alg::str::CONCATENATE_STRINGS<size_t>(cycle, [](size_t s) { return std::to_string(s); }) << "\n";
        }
    }

    bool DependencyManager::hasCircularDependency(std::vector<size_t>& currentPath) const {
        for (const auto extension : m_vertices.at(currentPath.back()).m_outEdges) {
            bool cycleFound = std::find(TT_IT(currentPath), extension) != currentPath.end();
            currentPath.push_back(extension);
            if (cycleFound || hasCircularDependency(currentPath)) {
                return true;
            }
            currentPath.pop_back();
        }
        return false;
    }

    bool DependencyManager::hasCircularDependency(size_t index) const {
        std::vector<size_t> currentPath = {index};
        return hasCircularDependency(currentPath);
    }

    void DependencyManager::clearUpdated() {
        m_updated.clear();
    }

    const std::set<size_t>& DependencyManager::updated() const {
        return m_updated;
    }

    std::vector<size_t> DependencyManager::unrollOrder() const {
        std::vector<size_t> partialOrder;
        for (const auto& [index, vertex] : m_vertices) {
            if (not hasCircularDependency(index)) {
                unrollOrder(index, partialOrder);
            }
        }
        for (auto it = partialOrder.begin(); it != partialOrder.end();) {
            if (m_updated.find(*it) == m_updated.end()) {
                it = partialOrder.erase(it);
            } else {
                ++it;
            }
        }
        return partialOrder;
    }

    void DependencyManager::unrollOrder(size_t start, std::vector<size_t>& partialOrder) const {
        if (std::find(TT_IT(partialOrder), start) != partialOrder.end()) {
            return;
        }
        const auto& vertex = m_vertices.at(start);
        for (const auto outEdge : vertex.m_outEdges) {
            unrollOrder(outEdge, partialOrder);
        }
        if (vertex.m_outEdges.size() == vertex.m_dependencies.size() &&
            std::all_of(TT_IT(vertex.m_outEdges), TT_LAMBDA_REF(a, return std::find(TT_IT(partialOrder), a) != partialOrder.end();))) {
            partialOrder.push_back(start);
        }
    }

    std::set<size_t> DependencyManager::unrollable() const {
        const auto order = unrollOrder();
        return {TT_IT(order)};
    }

    size_t DependencyManager::get(const FunctionToken& functionToken) {
        const auto it = std::find_if(TT_IT(m_vertices), TT_LAMBDA_REF(a, return gen::S_VARIANT_EQUALS(a.second.m_type, functionToken);));
        return it == m_vertices.end() ? std::numeric_limits<size_t>::max() : it->first;
    }

    size_t DependencyManager::get(const ConstantToken& constantToken) {
        const auto it = std::find_if(TT_IT(m_vertices), TT_LAMBDA_REF(a, return gen::S_VARIANT_EQUALS(a.second.m_type, constantToken);));
        return it == m_vertices.end() ? std::numeric_limits<size_t>::max() : it->first;
    }

    size_t DependencyManager::get(const Dependency& dependency) {
        return std::visit([this](const auto& a) { return get(a); }, dependency.get());
    }

    const DependencyManager::Vertex& DependencyManager::vertexAt(size_t index) const {
        assert(m_vertices.find(index) != m_vertices.end());
        return m_vertices.at(index);
    }

} // namespace ast
