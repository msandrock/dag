#include <cstdio>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "stdafx.hpp"
#include "dag.hpp"
#include "svg.hpp"

#ifdef TEST
#include "tests/test.hpp"
#endif

/**
 * Consume all lines from stdin and return a list of verified entries
 */
std::vector<std::string> parse_stdin() {
    std::vector<std::string> lines;
    auto lineNumber = 1;

    for (std::string line; std::getline(std::cin, line); lineNumber++) {
        trim(line);
        lines.push_back(line);
    }

    return lines;
}

/**
 * Handle segfaults and print a backtrace to stderr before exiting
 */
void shutdown_handler(int sig) {
    const size_t MAX_BACKTRACE_SIZE = 10;
    void *array[MAX_BACKTRACE_SIZE];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, MAX_BACKTRACE_SIZE);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(EXIT_FAILURE);
}

int main(int argc, const char** argv) {
    signal(SIGSEGV, shutdown_handler);
    // Check for command line parameters
    if (argc > 1 && std::string(argv[1]) == "-v") {
        std::cout << VERSION << std::endl;
        return EXIT_SUCCESS;
    }
#ifdef TEST
    run_all_tests();
    return EXIT_SUCCESS;
#endif
    // Collect parsed lines
    std::vector<std::string> lines = parse_stdin();
    // Convert parsed lines to dependency structs
    dag::dependency_vec dependencies = dag::convert_dependencies(lines);
    dag::node_vec startNodes;
    build_dag(dependencies, startNodes);

    //auto nodeCount = get_node_count(startNodes);
    //std::cout << "Created dag with " << nodeCount << " nodes" << std::endl;

    //std::cout << "#######" << std::endl;
    //std::cout << "# DAG #" << std::endl;
    //std::cout << "#######" << std::endl;
    //print_nodes(startNodes); 
    //std::cout << std::endl;

    write_svg(startNodes, "/tmp/dag.svg");
    system("open /tmp/dag.svg");
    //remove("/tmp/dag.svg");
    return EXIT_SUCCESS;
} 
