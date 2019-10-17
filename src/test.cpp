#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include "test.hpp"
#include "dag.hpp"

void test_add_single_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = {
        dag::Dependency("a", "b")
    };

    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> nodes;
    // Build dag
    dag::build_dag(&dependencies, &nodes);
    // Only one dag should be generated
    assert(nodes.size() == 1);
    // Count nodes
    std::set<size_t> accumulator; 
    dag::count_nodes(nodes[0], accumulator);

    assert(accumulator.size() == 2);
}


void run_all_tests() {

    // add test for convert_dependencies
    // add dag_test.cpp
    // add function to count nodes in dag

    std::cout << "Running tests" << std::endl;
    test_add_single_dependency();

    std::cout << "All tests complete" << std::endl;
}