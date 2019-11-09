#ifndef DAG_HPP
#define DAG_HPP
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace dag {
    struct DagNode;
    struct Dependency;
    typedef std::shared_ptr<DagNode> node_ptr;
    typedef std::vector<node_ptr> node_vec;
    typedef std::vector<Dependency> dependency_vec;

    struct Dependency {
        std::string name;
        std::string downstream;
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
    void build_dag(dependency_vec& dependencies, node_vec& startNodes);
    size_t get_node_count(const node_vec& startNodes);
    void print_nodes(node_vec nodes);
}
#endif