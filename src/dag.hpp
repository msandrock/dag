#pragma once
#include <set>
#include <string>
#include <vector>

namespace dag {
    class Dependency {
        size_t name_hash;
        size_t downstream_hash;
        std::string name;
        std::string downstream;

        public:
        Dependency(const std::string& name, const std::string& downstream);
        // Implement custom < operator to implement value compare
        /*bool operator< (const Dependency& other) const {
            size_t lhsVal = name_hash + downstream_hash;
            size_t rhsVal = other.getNameHash() + other.getDownstreamHash();
            return (lhsVal < rhsVal);
        }*/
        std::string getName() const { return name; }
        std::string getDownstream() const { return downstream; }
        size_t getNameHash() const { return name_hash; }
        size_t getDownstreamHash() const { return downstream_hash; }
    };

    struct DagNode {
        std::string name;
        size_t name_hash;
        std::vector<std::shared_ptr<DagNode>> ancestors;
        std::vector<std::shared_ptr<DagNode>> children;

        DagNode(const std::string& name);
        std::string getName() const { return name; }
        size_t getNameHash() const { return name_hash; }
    };

    std::vector<dag::Dependency> convert_dependencies(const std::vector<std::string>& lines);
    void build_dag(std::vector<Dependency>* dependencies, std::vector<std::shared_ptr<DagNode>>* startNodes);
    void count_nodes(std::shared_ptr<DagNode> node, std::set<size_t>& accumulator);
    void print_nodes(std::shared_ptr<DagNode> node, int level);
}
