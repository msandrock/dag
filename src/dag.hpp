#pragma once
#import <string>

struct Dependency {
    size_t ancestor_hash;
    size_t child_hash;

    std::string ancestor;
    std::string child;
};

struct DagNode {
    Dependency dependency;
    std::string name;
    std::vector<DagNode*> ancestors;
    std::vector<DagNode*> children;
};

std::vector<DagNode*> build_dag(const std::vector<Dependency>& dependencies);
void print_dag(const DagNode* startNode);
void print_node(const DagNode* node, int level);