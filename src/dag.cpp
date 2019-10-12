#include <iostream>
#include <string>
#include <vector>
#include "dag.hpp"

namespace dag {
    std::vector<DagNode> build_dag(const std::vector<Dependency>& dependencies) {
        std::vector<DagNode> startNodes;

        return startNodes;
    }

    void print_nodes(const DagNode& node, int level) {
        std::cout << std::string(level, '\t') << node.name << std::endl;

        for (const DagNode ancestor: node.ancestors) {
            print_nodes(ancestor, level + 1);
        }
    }
}
