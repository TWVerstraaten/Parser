//
// Created by pc on 03-06-21.
//

#include "DependencyGraph.h"

#include "Overloaded.h"

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <iostream>
namespace gen {

    typedef std::vector<boost::default_color_type> ColorMap;

    void DependencyGraph::addVertex(size_t index, const FunctionVertex::Type& type) {
        //        if (getVertexByIndex(index) != boost::graph_traits<Graph>::null_vertex()) {
        boost::add_vertex(FunctionVertex{index, type}, m_graph);
        //        }
    }

    void DependencyGraph::addDependsOn(const std::string& function, const std::string& dependsOn) {
        //        if (not edgeExists(function, dependsOn)) {
        //            add_edge(getOrAddVertexByName(dependsOn), getOrAddVertexByName(function), m_graph);
        //        }
    }

    DependencyGraph::Vertex DependencyGraph::getVertexByIndex(size_t index) {
        auto itPair = vertices(m_graph);
        for (auto it = itPair.first; it != itPair.second; ++it) {
            if (get(boost::vertex_name, m_graph)[*it].m_index == index) {
                return *it;
            }
        }
        return boost::graph_traits<Graph>::null_vertex();
    }

    struct DependentsVisitor : public boost::bfs_visitor<> {
        explicit DependentsVisitor(std::set<std::string>& dependents) : m_dependents(dependents) {
        }

        template <class Vertex, class Graph>
        void discover_vertex(Vertex v, Graph& g) {
            m_dependents.insert(get(boost::vertex_name, g)[v]);
        }
        std::set<std::string>& m_dependents;
    };

    //    std::set<std::string> DependencyGraph::getDependents(const std::string& function) {
    //        std::set<std::string> result;
    //        DependentsVisitor     vis(result);
    //        ColorMap              colorMap(num_vertices(m_graph), boost::white_color);
    //        breadth_first_visit(m_graph, getVertexByName(function), boost::visitor(vis).color_map(&colorMap[0]));
    //        return result;
    //    }

    struct CycleDetector : public boost::dfs_visitor<> {
        explicit CycleDetector(bool& hasCycle) : m_hasCycle(hasCycle) {
        }

        template <class Edge, class Graph>
        void back_edge(Edge, Graph&) {
            m_hasCycle = true;
        }

        bool& m_hasCycle;
    };

    //    bool DependencyGraph::hasCircularDependency(const std::string& function) {
    //        const auto    reversed = boost::reverse_graph<Graph, Graph&>(m_graph);
    //        bool          hasCircularDependency = false;
    //        CycleDetector vis(hasCircularDependency);
    //        ColorMap      colorMap(num_vertices(m_graph), boost::white_color);
    //        depth_first_visit(reversed, getVertexByName(function), vis, &colorMap[0]);
    //        return hasCircularDependency;
    //    }
    //
    //    DependencyGraph::Vertex DependencyGraph::getOrAddVertexByName(const std::string& string) {
    //        auto vertex = getVertexByName(string);
    //        return vertex != boost::graph_traits<Graph>::null_vertex() ? vertex : boost::add_vertex(string, m_graph);
    //    }

    void DependencyGraph::write() const {
        std::vector<std::string> names;
        names.resize(num_vertices(m_graph));
        const auto itPair = boost::vertices(m_graph);
        size_t     index  = 0;
        for (auto it = itPair.first; it != itPair.second; ++it, ++index) {
            names[index] = std::to_string(get(boost::vertex_name, m_graph)[*it].m_index) + " " +
                           std::visit(Overloaded{[](const std::string& a) { return a; },
                                                 [](const ast::par::FunctionToken& f) { return f.toString(); },
                                                 [](const auto& a) { return std::string("_"); }},
                                      get(boost::vertex_name, m_graph)[*it].m_type);
        }
        write_graphviz(std::cout, m_graph, boost::make_label_writer(&names[0]));
    }

    bool DependencyGraph::selfDependency(const std::string& function) {
        return edgeExists(function, function);
    }

    size_t DependencyGraph::edgeCount() const {
        return num_edges(m_graph);
    }

    bool DependencyGraph::edgeExists(const std::string& u, const std::string& v) {
        //        const auto uVertex = getVertexByName(u);
        //        const auto vVertex = getVertexByName(v);
        //        if (uVertex == boost::graph_traits<Graph>::null_vertex() || vVertex == boost::graph_traits<Graph>::null_vertex()) {
        //            return false;
        //        }
        //        return edge(uVertex, vVertex, m_graph).second;
        return false;
    }

} // namespace gen
