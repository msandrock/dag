#include <iostream>
#include <string>
#include <vector>
#include "dag.hpp"

namespace dag {

    Dependency::Dependency(const std::string& name, const std::string& downstream) {
        std::hash<std::string> hash_fn;
        this->name = name;
        this->downstream = downstream;
        this->name_hash = hash_fn(name);
        this->downstream_hash = hash_fn(downstream);
    }

    /**
     * Loop over all dependencies and find the upstream node
     */
    void find_upstream_dependency(Dependency* current, std::vector<Dependency>* dependencies, Dependency** found) {
        // TODO: Maybe add support to find multiple ancestors?
        int i = 0;
        for (auto dependency: *dependencies) {
            if (dependency.getDownstreamHash() == current->getNameHash()) {
                // Obtain pointer into vector
                *found = &(dependencies->data()[i]);
                return;
            }
            i++;
        }
    }

    /**
     * Append direct child nodes to the current node
     */ 
    void append_dependencies(std::shared_ptr<DagNode> currentNode, std::vector<Dependency>* dependencies) {
        // Find all nodes that have the current node as a parent
        for (auto dependency: *dependencies) {
            // No self-referencing
            if (dependency.getNameHash() == currentNode->dependency.getNameHash()) {
                continue;
            }

            Dependency* ancestor = nullptr;
            find_upstream_dependency(&dependency, dependencies, &ancestor);

            // Dependency has no ancestor
            if (ancestor == nullptr) {
                continue;
            }

            // Is the upstream node ancestor to the current node?
            if (ancestor->getNameHash() == currentNode->dependency.getNameHash()) {
                // Add the current dependency as a child node
                std::shared_ptr<DagNode> newNode(new DagNode(dependency));
                std::cout << "Add " << newNode->dependency.getName() << " to " << currentNode->dependency.getName() << std::endl;

                currentNode->children.push_back(newNode);
                newNode->ancestors.push_back(currentNode);

                append_dependencies(newNode, dependencies);
            }
        }
    }
    
    /**
     * Construct dag from the given dependencies
     */ 
    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes) {
        // 1. Every node that is not a child node is automatically a start node
        for (auto dependency: *dependencies) {
            Dependency* ancestor = nullptr;
            find_upstream_dependency(&dependency, dependencies, &ancestor);
            
            if (ancestor == nullptr) {
                std::shared_ptr<DagNode> startNode(new DagNode(dependency));
                startNodes->push_back(startNode);
            }
        }

        // Construct dags for all start nodes
        for (auto startNode: *startNodes) {
            // Pass in the current dag - add all nodes to the end nodes
            // dag, current node, dependencies
            append_dependencies(startNode, dependencies);
        }

        // Compare every node with the others - if it is not a child node, then it's a start node
            // once assigned, flag it as consumed

        // 2. Every node that references an ancestor node is invalid
    }

    /**
     * Print a text representation of the dag
     */
    void print_nodes(std::shared_ptr<DagNode> node, int level) {
        std::cout << std::string(level, '\t') << node->dependency.getName() << std::endl;
        std::cout << "downstream nodes: " << node->children.size() << std::endl;

        for (auto childNode: node->children) {
            print_nodes(childNode, level + 1);
        }
    }
}
