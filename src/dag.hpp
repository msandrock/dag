#pragma once
#include <set>
#include <string>
#include <vector>

namespace dag {
    struct Dependency {
        std::string name;
        std::set<std::string> downstreams;
    };

    struct DagNode {
        std::string name;
        std::vector<std::shared_ptr<DagNode>> ancestors;
        std::vector<std::shared_ptr<DagNode>> children;

        DagNode(const std::string& name);
        std::string getName() const { return name; }
    };

    std::vector<dag::Dependency> convert_dependencies(const std::vector<std::string>& lines);
    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes);
    void count_nodes(std::shared_ptr<DagNode> node, std::set<std::string>& accumulator);
    void print_nodes(std::shared_ptr<DagNode> node, int level);
}
