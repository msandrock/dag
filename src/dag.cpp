#include <iostream>
#include <string>
#include <vector>
#include "dag.hpp"

namespace dag {

    Dependency* find_ancestor(const Dependency& current, const std::vector<Dependency>& dependencies) {
        Dependency* ancestor = nullptr; 
        // Loop over all dependencies and find the ancestor node
        for(auto dependency: dependencies) {
            if (dependency.child_hash == current.ancestor_hash) {
                ancestor = &dependency;
                break;
            }
        }

        return ancestor;
    }
    
    std::vector<DagNode> build_dag(const std::vector<Dependency>& dependencies) {
        std::vector<DagNode> startNodes;
        // 1. Every node that is not a child node is automatically a start node
        for (auto dependency: dependencies) {
            auto ancestor = find_ancestor(dependency, dependencies);
            
            if (ancestor == nullptr) {
                DagNode startNode;
                startNode.dependency = dependency;
                startNodes.push_back(startNode);
            }
        }

        // Compare every node with the others - if it is not a child node, then it's a start node
            // once assigned, flag it as consumed

        // 2. Every node that references an ancestor node is invalid

        return startNodes;
    }

    void print_nodes(const DagNode& node, int level) {
        std::cout << std::string(level, '\t') << node.dependency.ancestor << std::endl;

        for (const DagNode ancestor: node.ancestors) {
            print_nodes(ancestor, level + 1);
        }
    }
}
