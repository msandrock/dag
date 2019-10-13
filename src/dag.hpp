#pragma once
#include <string>

namespace dag {
    struct Dependency {
        size_t ancestor_hash;
        size_t child_hash;
        std::string ancestor;
        std::string child;
    };

    struct DagNode {
        Dependency dependency;
        std::vector<DagNode> ancestors;
        std::vector<DagNode> children;
    };

    std::vector<DagNode> build_dag(const std::vector<Dependency>& dependencies);
    void print_nodes(const DagNode& node, int level);
}
