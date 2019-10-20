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
            // Ignore empty lines and comments
            if (line == "" || line[0] == '#') continue;

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
     * Add leaf node, if no other dependencies require the given node
     */
    bool _append_leaf_node(std::shared_ptr<DagNode> node, std::vector<Dependency>* dependencies, const Dependency& upstreamDependency) {
        // Find a dependency that has the given dependency as a downstream
        for (auto dependency: *dependencies) {
            // Is downstream to given dependency and has further downstreams
            if (dependency.name == upstreamDependency.downstream && dependency.downstream != "") {
                // No leaf added - return false
                return false;
            }
        }

        // If we reach this point, then there are no further dependencies for upstream dependency
        // - add leaf node
        std::shared_ptr<DagNode> downstreamNode(new DagNode(upstreamDependency.downstream));
        node->children.push_back(downstreamNode);
        downstreamNode->ancestors.push_back(node);

        // Leaf added - return true
        return true;
    }

    /**
     * Append direct child nodes to the current node
     */ 
    void _append_child_nodes(std::shared_ptr<DagNode> parentNode, std::vector<Dependency>* dependencies) {
        // Find all dependencies that have the current node as a parent
        int i = 0;
        for (auto dependency: *dependencies) {
            if (dependency.used) continue;

            // Find upstream node that points to the current dependency
            Dependency* upstream = nullptr;
            bool leafAdded = false;
            _find_upstream_dependency(dependency, dependencies, &upstream);

            if (dependency.name == parentNode->name && dependency.downstream != "") {
                // Check if we need to add the current downstream as a leaf node 
                leafAdded = _append_leaf_node(parentNode, dependencies, dependency);

                if (leafAdded) {
                    (*dependencies).data()[i].used++;
                }
            }

            // Dependency has no ancestor - it's a root dependency
            if (upstream == nullptr || upstream->name != parentNode->name) {
                i++;
                continue;
            }

            // Check if the current node has a child node for the given upstream yet
            std::shared_ptr<DagNode> childNode = nullptr;
            bool nodeExists = false;
            for (auto child: parentNode->children) {
                if (child->name == dependency.name) {
                    // Node already has a child node with the same name
                    childNode = child;
                    nodeExists = true;
                }
            }

            if (childNode == nullptr) {
                childNode = std::shared_ptr<DagNode>(new DagNode(dependency.name));
            }

            // If the downstream node is not an upstream dependency to another node, add it as a standalone node
            // Find nodes that have the downstream node as a dependency
            // e.g. a>b - b is not a dependency of any other node - add it as standalone
            // Find nodes that have 'c' as a parent

            if (!leafAdded) {
                leafAdded = _append_leaf_node(childNode, dependencies, dependency);
            }

            if (!leafAdded) {
                // Other nodes are referencing the downstream dependency - enter recursion
                _append_child_nodes(childNode, dependencies);
            }

            // Append new node to parent
            if (!nodeExists) {
                parentNode->children.push_back(childNode);
                childNode->ancestors.push_back(parentNode);
            }

            (*dependencies).data()[i].used++;
            i++;
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
            _append_child_nodes(startNode, dependencies);
        }

        // 2. Every node that references an ancestor node is invalid
    }

    /**
     * Recursively count the number of nodes in the dag
     */
    void _count_nodes(const DagNode& node, std::set<std::string>& accumulator) {
        // If the node is already in the set, do not add it again
        accumulator.insert(node.name);

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
        std::cout << std::string(level, '\t') << node->name << "[" << node->children.size() << "]" << std::endl;
        for (auto childNode: node->children) {
            print_nodes(childNode, level + 1);
        }
    }
}
