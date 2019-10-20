#ifndef DAG_HPP
#define DAG_HPP
#include <set>
#include <string>
#include <vector>

namespace dag {
    struct Dependency {
        std::string name;
        std::string downstream;
        int used;
    };

    struct DagNode {
        std::string name;
        std::vector<std::shared_ptr<DagNode>> ancestors;
        std::vector<std::shared_ptr<DagNode>> children;

        DagNode(const std::string& name);
        std::string getName() const { return name; }
    };

    Dependency convert_dependency(const std::string& line);
    std::vector<Dependency> convert_dependencies(const std::vector<std::string>& lines);
    void append_dependencies(std::shared_ptr<DagNode> currentNode, std::vector<Dependency>* dependencies);
    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes);
    size_t get_node_count(const DagNode& node);
    void print_nodes(std::shared_ptr<DagNode> node, int level);
}
#endif