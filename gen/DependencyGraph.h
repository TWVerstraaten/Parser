//
// Created by pc on 03-06-21.
//

#ifndef PARSER_DEPENDENCYGRAPH_H
#define PARSER_DEPENDENCYGRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <string>

namespace gen {

    class DependencyGraph {
        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, boost::property<boost::vertex_name_t, std::string>> Graph;

        typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

      public:
        void addVertex(const std::string& vertexName);
        void addDependsOn(const std::string& function, const std::string& dependsOn);
        void write() const;

        [[nodiscard]] size_t edgeCount() const;

        [[nodiscard]] std::set<std::string> getDependents(const std::string& function);
        [[nodiscard]] bool                  hasCircularDependency(const std::string& function);
        [[nodiscard]] bool                  selfDependency(const std::string& function);

      private:
        [[nodiscard]] bool edgeExists(const std::string& u, const std::string& v);

        Vertex getVertexByName(const std::string& string);
        Vertex getOrAddVertexByName(const std::string& string);

        Graph m_graph;
    };
} // namespace gen

#endif // PARSER_DEPENDENCYGRAPH_H
