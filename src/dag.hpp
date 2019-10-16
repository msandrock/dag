#pragma once
#include <string>

namespace dag {
    class Dependency {
        size_t name_hash;
        size_t downstream_hash;
        std::string name;
        std::string downstream;

        public:
        Dependency(const std::string& name, const std::string& downstream);

        std::string getName() const { return name; }
        std::string getDownstream() const { return downstream; }
        size_t getNameHash() const { return name_hash; }
        size_t getDownstreamHash() const { return downstream_hash; }
    };

    struct DagNode {
        Dependency dependency;
        std::vector<std::shared_ptr<DagNode>> ancestors;
        std::vector<std::shared_ptr<DagNode>> children;

        DagNode(const Dependency& dependency) : dependency(dependency) {}
    };

    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes);
    void print_nodes(std::shared_ptr<DagNode> node, int level);
}
