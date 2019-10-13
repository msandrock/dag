#include <iostream>
#include <string>
#include <vector>
#include "dag.hpp"

namespace dag {
    /**
     * Loop over all dependencies and find the upstream node
     */
    void find_upstream_dependency(const Dependency& current, std::vector<Dependency>& dependencies, Dependency** found) {
        // TODO: Maybe add support to find multiple ancestors?
        int i = 0;
        for (auto dependency: dependencies) {
            if (dependency.downstream_hash == current.name_hash) {
                // Obtain pointer into vector
                *found = &(dependencies.data()[i]);
                return;
            }
            i++;
        }
    }

    /**
     * Construct dag from the given dependencies
     */ 
    std::vector<DagNode> build_dag(std::vector<Dependency>& dependencies) {
        std::vector<DagNode> startNodes;
        // 1. Every node that is not a child node is automatically a start node
        for (auto dependency: dependencies) {
            Dependency* ancestor = nullptr;
            find_upstream_dependency(dependency, dependencies, &ancestor);
            
            if (ancestor == nullptr) {
                DagNode startNode;
                startNode.dependency = dependency;
                startNodes.push_back(startNode);
            }
        }

        // Construct dags for all start nodes
        for (auto startNode: startNodes) {
            std::cout << "Building dag for " << startNode.dependency.name << std::endl;
            // Find all nodes that have the start node as a parent
            for (auto dependency: dependencies) {
                // Skip root nodes
                if (dependency.name_hash == startNode.dependency.name_hash) {
                    continue;
                }

                std::cout << "Finding ancestor for " << dependency.name << std::endl;
                Dependency* ancestor = nullptr;
                find_upstream_dependency(dependency, dependencies, &ancestor);

                // Current node has an ancestor
                if (ancestor != nullptr) {
                    std::cout << "Found an ancestor for " << dependency.name << std::endl;
                    std::cout << (*ancestor).name_hash << " == " << startNode.dependency.name_hash << "?" << std::endl;

                    // Is the start node the ancestor?
                    /*if (ancestor->name == startNode.dependency.name) {
                        // Add the current node as a child
                        std::cout << dependency.name << " is a direct child of " << startNode.dependency.name << std::endl;
                    }*/
                } else {
                    std::cout << "Could not find an ancestor for " << dependency.name << std::endl;
                }
            }
        }


        // Compare every node with the others - if it is not a child node, then it's a start node
            // once assigned, flag it as consumed

        // 2. Every node that references an ancestor node is invalid

        return startNodes;
    }

    void print_nodes(const DagNode& node, int level) {
        std::cout << std::string(level, '\t') << node.dependency.name << std::endl;

        for (const DagNode ancestor: node.ancestors) {
            print_nodes(ancestor, level + 1);
        }
    }
}
