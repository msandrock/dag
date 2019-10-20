#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "dag.hpp"

namespace dag {
    /**
     * Instantiate a new dag node; Calculate and store name hash
     */
    DagNode::DagNode(const std::string& name) {
        this->name = name;
    }

    /**
     * Convert single line to dependency
     */
    Dependency convert_dependency(const std::string& line) {
        Dependency dependency{};
        const auto pos = line.find('>');

        if (pos != std::string::npos) {
            // Item has a child dependency
            dependency.name = line.substr(0, pos);
            dependency.downstream = line.substr(pos + 1);
        } else {
            dependency.name = line;
        }

        return dependency;
    }

    /**
     * Convert list of lines into dependency structs
     */
    std::vector<Dependency> convert_dependencies(const std::vector<std::string>& lines) {
        std::vector<Dependency> dependencies;

        for (auto line: lines) {
            auto dependency = convert_dependency(line);
            dependencies.push_back(dependency);
        }

        return dependencies;
    }

    /**
     * Loop over all dependencies and find the upstream node
     */
    void find_upstream_dependency(Dependency* current, std::vector<Dependency>* dependencies, Dependency** found) {
        // TODO: Maybe add support to find multiple ancestors?
        int i = 0;
        for (auto dependency: *dependencies) {
            if (dependency.downstream == current->name) {
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
            if (dependency.name == currentNode->getName()) {
                continue;
            }

            Dependency* ancestor = nullptr;
            find_upstream_dependency(&dependency, dependencies, &ancestor);

            // Dependency has no ancestor
            if (ancestor == nullptr) {
                continue;
            }

            // Is the upstream node ancestor to the current node?
            if (ancestor->name == currentNode->getName()) {
                // Add the current dependency as a child node
                std::shared_ptr<DagNode> newNode(new DagNode(dependency.name));
                std::cout << "Add " << newNode->getName() << " to " << currentNode->getName() << std::endl;

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
                std::shared_ptr<DagNode> startNode(new DagNode(dependency.name));
                startNodes->push_back(startNode);
            }
        }

        // When adding nodes to the dag, the dependencies have to be flagged
        // If the downstream node is not an upstream dependency to another node, add it as a standalone node
        // Find nodes that have the downstream node as a dependency
        // e.g. a>b - b is not a dependency of any other node - add it as standalone

        // Construct dags for all start nodes
        for (auto startNode: *startNodes) {
            // Pass in the current dag - add all nodes to the end nodes
            // dag, current node, dependencies
            append_dependencies(startNode, dependencies);
        }

        // 2. Every node that references an ancestor node is invalid
    }

    /**
     * Count the number of nodes in the dag
     */
    void count_nodes(std::shared_ptr<DagNode> node, std::set<std::string>& accumulator) {
        // If the node is already in the set, do not add it again
        accumulator.insert(node->getName());

        for (auto child: node->children) {
            count_nodes(child, accumulator);
        }
    }

    /**
     * Print a text representation of the dag
     */
    void print_nodes(std::shared_ptr<DagNode> node, int level) {
        std::cout << std::string(level, '\t') << node->getName() << std::endl;
        std::cout << "downstream nodes: " << node->children.size() << std::endl;

        for (auto childNode: node->children) {
            print_nodes(childNode, level + 1);
        }
    }
}
