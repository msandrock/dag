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
     * Append direct child nodes to the current node
     */ 
    void append_dependencies(DagNode& currentNode, std::vector<Dependency>& dependencies) {
        // Find all nodes that have the current node as a parent
        for (auto dependency: dependencies) {
            // No self-referencing
            if (dependency.name_hash == currentNode.dependency.name_hash) {
                continue;
            }

            Dependency* ancestor = nullptr;
            find_upstream_dependency(dependency, dependencies, &ancestor);

            // Dependency has no ancestor
            if (ancestor == nullptr) {
                continue;
            }

            // Is the start node the ancestor?
            if (ancestor->name_hash == currentNode.dependency.name_hash) {
                // Add the current node as a child
                std::cout << dependency.name << " is a direct child of " << currentNode.dependency.name << std::endl;
            }
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
            // Pass in the current dag - add all nodes to the end nodes
            // dag, current node, dependencies
            append_dependencies(startNode, dependencies);
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
