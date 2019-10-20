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
#include "test.hpp"
#endif

/**
 * Verify entries - throw exception on errors
 */
void verify_format(const std::string& line, int lineNumber) {
    // Make sure there is exactly one '>'
    //auto pos = line.find('>');

    //if (pos == std::string::npos) {
    //    throw ParseException("The line does not contain a '>'", line, lineNumber);
    //}

    //// Make sure there are no additional '>'
    //if (line.find('>', pos + 1) != std::string::npos) {
    //    throw ParseException("The line must contain exactly one '>'", line, lineNumber);
    //}
}

/**
 * Consume all lines from stdin and return a list of verified entries
 */
std::vector<std::string> parse_stdin() {
    std::vector<std::string> lines;
    auto lineNumber = 1;

    for (std::string line; std::getline(std::cin, line); lineNumber++) {
        trim(line);
        verify_format(line, lineNumber);
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
#ifdef TEST
    run_all_tests();
    return EXIT_SUCCESS;
#endif
    // Collect parsed lines
    std::vector<std::string> lines; 
    try {
        lines = parse_stdin();
    } catch(ParseException e) {
        std::cout << e.getMessage() << std::endl;
        return EXIT_FAILURE;
    }

    for (auto line: lines) {
        std::cout << "Line: " << line << std::endl;
    }

    // Convert parsed lines to dependency structs
    std::vector<dag::Dependency> dependencies = dag::convert_dependencies(lines);
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    build_dag(&dependencies, &startNodes);

    for (auto startNode: startNodes) {
        print_nodes(startNode, 0);
    }

    write_svg(startNodes, "dag.svg");
    system("open dag.svg");

    return EXIT_SUCCESS;
} 
