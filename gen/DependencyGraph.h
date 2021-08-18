//
// Created by pc on 03-06-21.
//

#ifndef GEN_DEPENDENCYGRAPH_H
#define GEN_DEPENDENCYGRAPH_H

#include "../ast/par/FunctionToken.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <string>
#include <variant>
namespace gen {

    class DependencyGraph {

        struct FunctionVertex {
            struct Anonymous {};
            typedef std::variant<ast::par::FunctionToken, std::string, Anonymous> Type;

            size_t m_index;
            Type   m_type;
        };

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, boost::property<boost::vertex_name_t, FunctionVertex>> Graph;
        typedef boost::graph_traits<Graph>::vertex_descriptor                                                                                 Vertex;

      public:
        void addVertex(size_t index, const FunctionVertex::Type& type = FunctionVertex::Anonymous());
        void addDependsOn(const std::string& function, const std::string& dependsOn);
        void write() const;

        [[nodiscard]] size_t edgeCount() const;

//        [[nodiscard]] std::set<std::string> getDependents(const std::string& function);
//        [[nodiscard]] bool                  hasCircularDependency(const std::string& function);
        [[nodiscard]] bool                  selfDependency(const std::string& function);

      private:
        [[nodiscard]] bool edgeExists(const std::string& u, const std::string& v);

        Vertex getVertexByIndex(size_t index);
//        Vertex getOrAddVertexByName(const std::string& string);

        Graph m_graph;
    };
} // namespace gen

#endif // GEN_DEPENDENCYGRAPH_H
