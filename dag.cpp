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
     * Loop over all dependencies and find the upstream node; Returns a pointer to the found node that references the dependency
     */
    void _find_upstream_dependency(const Dependency& current, std::vector<Dependency>* dependencies, Dependency** found) {
        // TODO: Maybe add support to find multiple ancestors?
        *found = nullptr;
        int i = 0;
        for (auto dependency: *dependencies) {
            if (dependency.downstream == current.name) {
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
    void _append_dependencies(std::shared_ptr<DagNode> currentNode, std::vector<Dependency>* dependencies) {
        // Find all dependencies that have the current node as a parent
        for (auto dependency: *dependencies) {
            // No self-referencing
            //if (dependency.name == currentNode->getName()) {
            //    continue;
            //}

            // Find upstream node that points to the current dependency
            Dependency* upstream = nullptr;
            _find_upstream_dependency(dependency, dependencies, &upstream);

            if (dependency.name == currentNode->getName() && dependency.downstream != "") {
                // TODO: Refactor this block to a "insert_leaf_node" function

                // Check if the downstream has downstream dependencies
                bool found = false;
                for (auto downstreamDependency: *dependencies) {
                    if (downstreamDependency.name == dependency.downstream && downstreamDependency.downstream != "") {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    std::shared_ptr<DagNode> downstreamNode(new DagNode(dependency.downstream));
                    //std::cout << "Add " << downstreamNode->getName() << " to " << currentNode->getName() << std::endl;
                    currentNode->children.push_back(downstreamNode);
                    downstreamNode->ancestors.push_back(currentNode);
                }
            }

            // Dependency has no ancestor - it's a root dependency
            if (upstream == nullptr) {
                continue;
            }

            /*
            currentNode = a
            upstream = a>b -> ignored - upstream is null
            upstream = b>c
            */

            if (upstream->name == currentNode->getName()) {
                // Add the current dependency as a child node
                std::shared_ptr<DagNode> newNode(new DagNode(dependency.name));
                //std::cout << "Add " << newNode->getName() << " to " << currentNode->getName() << std::endl;

                // If the downstream node is not an upstream dependency to another node, add it as a standalone node
                // Find nodes that have the downstream node as a dependency
                // e.g. a>b - b is not a dependency of any other node - add it as standalone
                // Find nodes that have 'c' as a parent

                //std::cout << "Fing downstream nodes to " << dependency.downstream << std::endl;
                bool found = false;
                for (auto downstreamDependency: *dependencies) {
                    if (downstreamDependency.name == dependency.downstream && downstreamDependency.downstream != "") {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    std::shared_ptr<DagNode> downstreamNode(new DagNode(dependency.downstream));
                    //std::cout << "Add " << downstreamNode->getName() << " to " << newNode->getName() << std::endl;
                    newNode->children.push_back(downstreamNode);
                    downstreamNode->ancestors.push_back(newNode);
                } else {
                    //std::cout << "Node " << dependency.downstream << " is a dependency" << std::endl;
                    // Other nodes are referencing the downstream dependency - enter recursion
                    _append_dependencies(newNode, dependencies);
                }

                // When adding nodes to the dag, the dependencies have to be flagged
                currentNode->children.push_back(newNode);
                newNode->ancestors.push_back(currentNode);
            }
        }
    }
    
    /**
     * Construct dag from the given dependencies
     */ 
    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes) {
        // 1. Every node that is not a child node is automatically a start node
        int i = 0;
        for (auto dependency: *dependencies) {
            Dependency* upstream = nullptr;
            _find_upstream_dependency(dependency, dependencies, &upstream);
            
            if (upstream == nullptr) {
                std::shared_ptr<DagNode> startNode(new DagNode(dependency.name));
                startNodes->push_back(startNode);
                // Set the use flag, if the dependency has no downstreams
                if (dependency.downstream == "") {
                    dependencies->data()[i].used++;
                }
            }

            i++;
        }

        // Construct dags for all start nodes
        for (auto startNode: *startNodes) {
            // Pass in the current dag - add all nodes to the end nodes
            // dag, current node, dependencies
            _append_dependencies(startNode, dependencies);
        }

        // 2. Every node that references an ancestor node is invalid
    }

    /**
     * Recursively count the number of nodes in the dag
     */
    void _count_nodes(const DagNode& node, std::set<std::string>& accumulator) {
        // If the node is already in the set, do not add it again
        accumulator.insert(node.getName());

        for (auto child: node.children) {
            _count_nodes(*child, accumulator);
        }
    }

    /**
     * Count the number of nodes in the dag
     */
    size_t get_node_count(const DagNode& node) {
        std::set<std::string> accumulator; 
        _count_nodes(node, accumulator);

        return accumulator.size();
    }

    /**
     * Print a text representation of the dag
     */
    void print_nodes(std::shared_ptr<DagNode> node, int level) {
        std::cout << std::string(level, '\t') << node->getName() << "[" << node->children.size() << "]" << std::endl;
        for (auto childNode: node->children) {
            print_nodes(childNode, level + 1);
        }
    }
}
