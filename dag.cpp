#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "dag.hpp"
#include "stdafx.hpp"

namespace dag {
    /**
     * Instantiate a new dag node; Calculate and store name hash
     */
    DagNode::DagNode(const std::string& name) {
        this->name = name;
        this->x = -1;
        this->y = -1;
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

        trim(dependency.name);
        trim(dependency.downstream);

        return dependency;
    }

    /**
     * Find a particular node in the dag
     */
    void _find_node(const std::string& needle, const node_vec& nodes, node_ptr* found) {
        // Loop over all nodes
        for (auto node: nodes) {
            // Is the current node the one we're looking for?
            if (node->name == needle) {
                *found = node;
                return;
            }

            // Search in child nodes
            _find_node(needle, node->children, found);
            if (*found != nullptr) {
                return;
            }
        }
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
    void _find_upstream_dependencies(const Dependency& needle, dependency_vec& dependencies, dependency_vec& found) {
        for (auto dependency: dependencies) {
            if (dependency.downstream == needle.name) {
                found.push_back(dependency);
            }
        }
    }

    /**
     * Add leaf node, if no other dependencies require the given node
     */
    bool _append_leaf_node(node_vec& startNodes, node_ptr node, const dependency_vec& dependencies, const Dependency& upstreamDependency) {
        // Find a dependency that has the given dependency as a downstream
        for (auto dependency: dependencies) {
            // Is downstream to given dependency and has further downstreams
            if (dependency.name == upstreamDependency.downstream && dependency.downstream != "") {
                // No leaf added - return false
                return false;
            }
        }

        // If we reach this point, then there are no further dependencies for upstream dependency
        // - add leaf node

        // Find the leaf node in the dag - make sure it has not been added before
        node_ptr downstreamNode(nullptr);
        _find_node(upstreamDependency.downstream, startNodes, &downstreamNode);

        if (downstreamNode == nullptr) {
            downstreamNode = std::make_shared<DagNode>(upstreamDependency.downstream);
        }

        node->children.push_back(downstreamNode);
        downstreamNode->ancestors.push_back(node);

        // Leaf added - return true
        return true;
    }

    /**
     * Append direct child nodes to the current node
     */ 
    void _append_child_nodes(node_vec& startNodes, node_ptr parentNode, dependency_vec& dependencies) {
        // Find all dependencies that have the current node as a parent
        int i = 0;
        for (auto dependency: dependencies) {
            // Find upstream nodes that point to the current dependency
            dependency_vec upstreams;
            _find_upstream_dependencies(dependency, dependencies, upstreams);
            bool leafAdded = false;

            if (dependency.name == parentNode->name && dependency.downstream != "") {
                // Check if we need to add the current downstream as a leaf node 
                leafAdded = _append_leaf_node(startNodes, parentNode, dependencies, dependency);

                if (leafAdded) {
                    dependencies.data()[i].used++;
                }
            }

            // Check if any of the upstreams points to the current node
            bool hasMatchingUpstream = false;
            for (auto upstream: upstreams) {
                if (upstream.name == parentNode->name) {
                    hasMatchingUpstream = true;
                    break;
                }
            }

            // Dependency has no ancestor - it's a root dependency
            if (!hasMatchingUpstream) {
                i++;
                continue;
            }

            // Check if the current node has a child node for the given upstream yet
            node_ptr childNode(nullptr);
            bool nodeExists = true;
            _find_node(dependency.name, startNodes, &childNode);

            if (childNode == nullptr) {
                nodeExists = false;
                childNode = std::make_shared<DagNode>(dependency.name);
            }

            if (!leafAdded) {
                leafAdded = _append_leaf_node(startNodes, childNode, dependencies, dependency);

                if (leafAdded) {
                    dependencies.data()[i].used++;
                }
            }

            // Append new node to parent
            parentNode->children.push_back(childNode);
            childNode->ancestors.push_back(parentNode);

            if (!leafAdded) {
                // Other nodes are referencing the downstream dependency - enter recursion
                _append_child_nodes(startNodes, childNode, dependencies);
            }

            dependencies.data()[i].used++;
            i++;
        }
    }

    /**
     * Find all start nodes in the graph and add then to the collection
     */
    void _append_start_nodes(dependency_vec& dependencies, node_vec& startNodes) {
        // 1. Every node that is not a child node is automatically a start node
        int i = 0;
        for (auto dependency: dependencies) {
            dependency_vec upstreams;
            _find_upstream_dependencies(dependency, dependencies, upstreams);
            // Dependency has upstream nodes - it's not a start node 
            if (upstreams.size()) {
                i++;
                continue;
            }

            // We have two dependencies a>b and a>c - both qualify as start nodes. We only want to add one start node for 'a'
            // Check if a start node with the same name exists, before adding a new one.
            bool found = false;
            for (auto node: startNodes) {
                if (node->name == dependency.name) {
                    found = true;
                }
            }

            // Start node already added - skip
            if (found) continue;
                
            node_ptr startNode = std::make_shared<DagNode>(dependency.name);
            startNodes.push_back(startNode);
            // Set the use flag, if the dependency has no downstreams
            if (dependency.downstream == "") {
                dependencies.data()[i].used++;
            }

            i++;
        }
    }

    /**
     * Assign positions to child nodes
     */
    void _calculate_child_positions(node_ptr parentNode, int x, int y) {
        // Update all child nodes
        for (auto node: parentNode->children) {
            if (node->x == -1) {
                node->x = x;
                node->y = y++;
            }

            //std::cout << "set " << node->name << " to " << node->x << "/" << node->y << std::endl;
            _calculate_child_positions(node, x + 1, y);
        }
    }

    /**
     * Assign x and y positions to dag nodes
     */
    void _calculate_positions(node_vec& startNodes) {
        // Iterate over the dag and compute the node indentations. 
        // If the indentation is greater than before, update it.
        int x = 0, y = 0;

        for (auto startNode: startNodes) {
            startNode->x = x;
            startNode->y = y;

            // Assign coordinates for nodes
            _calculate_child_positions(startNode, x + 1, y);
            y++;
        }
    }

    /**
     * Construct dag from the given dependencies
     */ 
    void build_dag(dependency_vec& dependencies, node_vec& startNodes) {
        // 1. Every node that is not a child node is automatically a start node
        _append_start_nodes(dependencies, startNodes);
        
        // Construct dags for all start nodes
        for (auto startNode: startNodes) {
            // Pass in the current dag - add all nodes to the end nodes
            // dag, current node, dependencies
            _append_child_nodes(startNodes, startNode, dependencies);
        }

        // TODO: Detect circular dependencies
        // 2. Every node that references an ancestor node is invalid
        _calculate_positions(startNodes);
    }

    /**
     * Recursively count the number of nodes in the dag
     */
    void _count_nodes(node_ptr node, std::unordered_set<node_ptr>& accumulator) {
        // If the node is already in the set, do not add it again
        accumulator.insert(node);

        for (auto child: node->children) {
            _count_nodes(child, accumulator);
        }
    }

    /**
     * Count the number of nodes in the dag
     */
    size_t get_node_count(const node_vec& startNodes) {
        // This uses the std::hash<std::unique_ptr> implementation
        // to calculate an unique hash for the node
        std::unordered_set<node_ptr> accumulator;

        for (auto startNode: startNodes) {
            _count_nodes(startNode, accumulator);
        }

        return accumulator.size();
    }

    /**
     * Recursively print node and its child nodes
     */
    void _print_child_nodes(node_ptr node, int level) {
        std::cout << std::string(level, '\t') << node->name << "[" << node->children.size() << "](" << node->x << "|" << node->y << ")" << std::endl;
        
        for (auto childNode: node->children) {
            _print_child_nodes(childNode, level + 1);
        }
    }

    /**
     * Print a text representation of the dag
     */
    void print_nodes(node_vec nodes) {
        for (auto node: nodes) {
            _print_child_nodes(node, 0);
        }
    }
}
