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
        std::vector<DagNode> ancestors;
        std::vector<DagNode> children;
    };

    std::vector<DagNode> build_dag(std::vector<Dependency>& dependencies);
    void print_nodes(const DagNode& node, int level);
}
