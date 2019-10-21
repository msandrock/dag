#ifndef DAG_HPP
#define DAG_HPP
#include <set>
#include <string>
#include <vector>

namespace dag {
    struct DagNode;
    typedef std::shared_ptr<DagNode> node_ptr;
    typedef std::vector<node_ptr> node_vec;

    struct Dependency {
        std::string name;
        std::string downstream;
        int used;
    };

    struct DagNode {
        std::string name;
        node_vec ancestors;
        node_vec children;
        int x;
        int y;

        DagNode(const std::string& name);
    };

    Dependency convert_dependency(const std::string& line);
    std::vector<Dependency> convert_dependencies(const std::vector<std::string>& lines);
    void build_dag(std::vector<Dependency>* dependencies, node_vec* startNodes);
    size_t get_node_count(const node_vec& startNodes);
    void print_nodes(node_ptr node, int level);
}
#endif