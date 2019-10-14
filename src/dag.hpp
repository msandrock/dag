#pragma once
#include <string>

namespace dag {
    struct Dependency {
        size_t name_hash;
        size_t downstream_hash;
        std::string name;
        std::string downstream;
    };

    struct DagNode {
        Dependency dependency;
        std::vector<std::shared_ptr<DagNode>> ancestors;
        std::vector<std::shared_ptr<DagNode>> children;
    };

    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes);
    void print_nodes(std::shared_ptr<DagNode> node, int level);
}
