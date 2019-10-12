#import <iostream>
#import <string>
#import <vector>
#import "dag.hpp"

std::vector<DagNode*> build_dag(const std::vector<Dependency>& dependencies) {
    std::vector<DagNode*> startNodes;

    return startNodes;
}

void print_dag(const DagNode* startNode) {

}

void print_node(const DagNode* node, int level) {
    std::cout << std::string(level, '\t') << node->name << std::endl;
}