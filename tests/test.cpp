#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include "test.hpp"
#include "../dag.hpp"

void _test_convert_dependencies() {
    {
        // Single item without dependencies
        std::vector<std::string> lines;
        lines.push_back("a");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 1);
        assert(dependencies[0].name == "a");
        assert(dependencies[0].downstream == "");
        assert(dependencies[0].used == 0);
    }
    {
        // Simple dependency pair
        std::vector<std::string> lines;
        lines.push_back("a>b");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 1);
        assert(dependencies[0].name == "a");
        assert(dependencies[0].downstream == "b");
    }
    {
        // White space removal
        std::vector<std::string> lines;
        lines.push_back(" a> b ");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 1);
        assert(dependencies[0].name == "a");
        assert(dependencies[0].downstream == "b");
    }
    {
        // Ignore empty lines and comments
        std::vector<std::string> lines;
        lines.push_back("a>b");
        lines.push_back("");
        lines.push_back("# Just a comment");
        lines.push_back("b>c");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 2);
        assert(dependencies[0].name == "a");
        assert(dependencies[0].downstream == "b");
        assert(dependencies[1].name == "b");
        assert(dependencies[1].downstream == "c");
    }
}

void _test_add_standalone_node() {
    // Create dependencies for test with no downstream
    dag::Dependency deps[] = { dag::Dependency { "a" } };
    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    // Build dag
    dag::build_dag(&dependencies, &startNodes);
    // The dependency should be marked as used
    assert(dependencies[0].used == 1);
    // The dag should consist of the node 'a'
    assert(get_node_count(startNodes) == 1);
    assert(startNodes[0]->name == "a");
}

void _test_add_single_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = { dag::Dependency { "a", "b" } };
    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    // Build dag
    dag::build_dag(&dependencies, &startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    //assert(dependencies[0].used == 1);

    // The dag should consist of the two nodes 'a' and 'b'
    assert(get_node_count(startNodes) == 2);
}

void _test_add_double_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = {
        dag::Dependency { "a", "b" },
        dag::Dependency { "b", "c" }
    };
    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    // Build dag
    dag::build_dag(&dependencies, &startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    //assert(dependencies[0].used == 1);

    // The dag should consist of the nodes 'a', 'b' and 'c'
    assert(get_node_count(startNodes) == 3);
}

void _test_add_reversed_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = {
        dag::Dependency { "b", "c" },
        dag::Dependency { "a", "b" }
    };
    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    // Build dag
    dag::build_dag(&dependencies, &startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    //assert(dependencies[0].used == 1);

    // The dag should consist of the nodes 'a', 'b' and 'c'
    assert(get_node_count(startNodes) == 3);
}

void _test_dependency_recombine() {
    /*
    a>b b>d
    a>c c>d
  
        /---b---\
    a---         ---d
        \---c---/
    */

    // Create dependencies for test
    dag::Dependency deps[] = {
        dag::Dependency { "a", "b" },
        dag::Dependency { "a", "c" },
        dag::Dependency { "b", "d" },
        dag::Dependency { "c", "d" }
    };
    std::vector<dag::Dependency> dependencies(std::begin(deps), std::end(deps));
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    // Build dag
    dag::build_dag(&dependencies, &startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    //assert(dependencies[0].used == 1);
    assert(get_node_count(startNodes) == 4);
}

// TODO: Write test for findnode

void run_all_tests() {
    std::cout << "Running tests" << std::endl;
    _test_convert_dependencies();
    _test_add_standalone_node();
    _test_add_single_dependency();
    _test_add_double_dependency();
    _test_add_reversed_dependency();
    _test_dependency_recombine();
    std::cout << "All tests complete" << std::endl;
}