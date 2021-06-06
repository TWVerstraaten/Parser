//
// Created by pc on 03-06-21.
//

#include "DependencyGraph.h"

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <iostream>

namespace gen {

    typedef std::vector<boost::default_color_type> ColorMap;

    void DependencyGraph::addVertex(const std::string& vertexName) {
        if (getVertexByName(vertexName) != boost::graph_traits<Graph>::null_vertex()) {
            boost::add_vertex(vertexName, m_graph);
        }
    }

    void DependencyGraph::addDependsOn(const std::string& dependent, const std::string& function) {
        if (not edgeExists(dependent, function)) {
            add_edge(getOrAddVertexByName(function), getOrAddVertexByName(dependent), m_graph);
        }
    }

    DependencyGraph::Vertex DependencyGraph::getVertexByName(const std::string& string) {
        auto itPair = vertices(m_graph);
        for (auto it = itPair.first; it != itPair.second; ++it) {
            if (get(boost::vertex_name, m_graph)[*it] == string) {
                return *it;
            }
        }
        return boost::graph_traits<Graph>::null_vertex();
    }

    struct dependents_visitor : public boost::bfs_visitor<> {
        explicit dependents_visitor(std::set<std::string>& dependents) : m_dependents(dependents) {
        }

        template <class Vertex, class Graph>
        void discover_vertex(Vertex v, Graph& g) {
            m_dependents.insert(get(boost::vertex_name, g)[v]);
        }
        std::set<std::string>& m_dependents;
    };

    std::set<std::string> DependencyGraph::getDependents(const std::string& function) {
        std::set<std::string> result;
        dependents_visitor    vis(result);
        ColorMap              colorMap(num_vertices(m_graph), boost::white_color);
        breadth_first_visit(m_graph, getVertexByName(function), boost::visitor(vis).color_map(&colorMap[0]));
        return result;
    }

    struct cycle_detector : public boost::dfs_visitor<> {
        explicit cycle_detector(bool& has_cycle) : m_hasCycle(has_cycle) {
        }

        template <class Edge, class Graph>
        void back_edge(Edge, Graph&) {
            m_hasCycle = true;
        }

        bool& m_hasCycle;
    };

    bool DependencyGraph::hasCircularDependency(const std::string& function) {
        const auto     reversed = boost::reverse_graph<Graph, Graph&>(m_graph);
        bool           hasCycle = false;
        cycle_detector vis(hasCycle);
        ColorMap       colorMap(num_vertices(m_graph), boost::white_color);
        depth_first_visit(reversed, getVertexByName(function), vis, &colorMap[0]);
        return hasCycle;
    }

    DependencyGraph::Vertex DependencyGraph::getOrAddVertexByName(const std::string& string) {
        auto vertex = getVertexByName(string);
        return vertex != boost::graph_traits<Graph>::null_vertex() ? vertex : boost::add_vertex(string, m_graph);
    }

    void DependencyGraph::write() const {
        std::vector<std::string> names;
        names.resize(num_vertices(m_graph));
        const auto itPair = boost::vertices(m_graph);
        size_t     index  = 0;
        for (auto it = itPair.first; it != itPair.second; ++it, ++index) {
            names[index] = get(boost::vertex_name, m_graph)[*it];
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
        const auto uVertex = getVertexByName(u);
        const auto vVertex = getVertexByName(v);
        if (uVertex == boost::graph_traits<Graph>::null_vertex() || vVertex == boost::graph_traits<Graph>::null_vertex()) {
            return false;
        }
        return edge(uVertex, vVertex, m_graph).second;
    }

} // namespace gen
