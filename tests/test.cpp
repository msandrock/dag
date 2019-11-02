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
        // Multiple dependencies with separator
        std::vector<std::string> lines;
        lines.push_back("a>b,a>c,b>d, c> d");
        auto dependencies = dag::convert_dependencies(lines);
        assert(dependencies.size() == 4);
        assert(dependencies[0].name == "a");
        assert(dependencies[0].downstream == "b");
        assert(dependencies[1].name == "a");
        assert(dependencies[1].downstream == "c");
        assert(dependencies[2].name == "b");
        assert(dependencies[2].downstream == "d");
        assert(dependencies[3].name == "c");
        assert(dependencies[3].downstream == "d");
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
    dag::dependency_vec dependencies(std::begin(deps), std::end(deps));
    dag::node_vec startNodes;
    // Build dag
    dag::build_dag(dependencies, startNodes);
    // The dag should consist of the node 'a'
    assert(get_node_count(startNodes) == 1);
    assert(startNodes[0]->name == "a");
}

void _test_add_single_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = { dag::Dependency { "a", "b" } };
    dag::dependency_vec dependencies(std::begin(deps), std::end(deps));
    dag::node_vec startNodes;
    // Build dag
    dag::build_dag(dependencies, startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    // The dag should consist of the two nodes 'a' and 'b'
    assert(get_node_count(startNodes) == 2);
}

void _test_add_double_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = {
        dag::Dependency { "a", "b" },
        dag::Dependency { "b", "c" }
    };
    dag::dependency_vec dependencies(std::begin(deps), std::end(deps));
    dag::node_vec startNodes;
    // Build dag
    dag::build_dag(dependencies, startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    // The dag should consist of the nodes 'a', 'b' and 'c'
    assert(get_node_count(startNodes) == 3);
}

void _test_add_reversed_dependency() {
    // Create dependencies for test
    dag::Dependency deps[] = {
        dag::Dependency { "b", "c" },
        dag::Dependency { "a", "b" }
    };
    dag::dependency_vec dependencies(std::begin(deps), std::end(deps));
    dag::node_vec startNodes;
    // Build dag
    dag::build_dag(dependencies, startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
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
    dag::dependency_vec dependencies(std::begin(deps), std::end(deps));
    dag::node_vec startNodes;
    // Build dag
    dag::build_dag(dependencies, startNodes);
    // Only one dag should be generated
    assert(startNodes.size() == 1);
    assert(get_node_count(startNodes) == 4);
}

// TODO: Write test for findnode

void _test_dependency_rearrange() {
    // Make sure nodes are arranged, so no dependencies go from right to left
    dag::Dependency deps[] = {
        dag::Dependency { "a", "b" },
        dag::Dependency { "a", "c" },
        dag::Dependency { "b", "e" },
        dag::Dependency { "c", "d" },
        dag::Dependency { "d", "e" }
    };
    dag::dependency_vec dependencies(std::begin(deps), std::end(deps));
    dag::node_vec startNodes;

    dag::build_dag(dependencies, startNodes);
    
    // Level 1
    assert(startNodes[0]->name == "a");
    assert(startNodes[0]->x == 0);
    // Level 2
    assert(startNodes[0]->children[0]->name == "b");
    assert(startNodes[0]->children[0]->x == 1);
    assert(startNodes[0]->children[1]->name == "c");
    assert(startNodes[0]->children[1]->x == 1);
    // Level 3:
    assert(startNodes[0]->children[1]->children[0]->name == "d");
    assert(startNodes[0]->children[1]->children[0]->x == 2);
    // Level 4 (This node should be shifted by one level)
    assert(startNodes[0]->children[0]->children[0]->name == "e");
    assert(startNodes[0]->children[0]->children[0]->x == 3);
}

void run_all_tests() {
    std::cout << "Running tests" << std::endl;
    _test_convert_dependencies();
    _test_add_standalone_node();
    _test_add_single_dependency();
    _test_add_double_dependency();
    _test_add_reversed_dependency();
    _test_dependency_recombine();
    _test_dependency_rearrange();
    std::cout << "All tests complete" << std::endl;
}