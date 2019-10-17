#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include "test.hpp"
#include "dag.hpp"

void test_convert_dependencies() {
    {
        // Single item without dependencies
        std::vector<std::string> lines;
        lines.push_back("a");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 1);
        assert(dependencies[0].name == "a");
        assert(dependencies[0].downstreams.size() == 0);
    }
    {
        std::vector<std::string> lines;
        lines.push_back("a>b");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 2);
    }
}

/*void test_add_single_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = { dag::Dependency("a", "b") };
    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> nodes;
    // Build dag
    dag::build_dag(&dependencies, &nodes);
    // Only one dag should be generated
    assert(nodes.size() == 1);
    // Count nodes
    std::set<std::string> accumulator; 
    dag::count_nodes(nodes[0], accumulator);
    // The dag should consist of the two nodes 'a' and 'b'
    assert(accumulator.size() == 2);
}*/

void run_all_tests() {
    std::cout << "Running tests" << std::endl;
    test_convert_dependencies();
    //test_add_single_dependency();
    std::cout << "All tests complete" << std::endl;
}