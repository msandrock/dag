#include <iostream> // std::cout
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "stdafx.hpp"
#include "dag.hpp"

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
 * Convert list of lines into dependency structs
 */
std::vector<dag::Dependency> convert_dependencies(const std::vector<std::string>& lines) {
    std::vector<dag::Dependency> dependencies;

    for (auto line: lines) {
        // Convert to dependency struct
        // lowercase and hash
        //std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
        const size_t pos = line.find('>');
        std::string name;
        std::string downstream;

        if (pos != std::string::npos) {
            // Item has a child dependency
            name = line.substr(0, pos);
            downstream = line.substr(pos + 1);
        } else {
            // Item is a standalone item without dependency
            name = line;
            downstream = "";
        }

        dependencies.push_back(dag::Dependency(name, downstream));
    }

    return dependencies;
}

int main(int argc, const char** argv) {
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
    std::vector<dag::Dependency> dependencies = convert_dependencies(lines);
    std::vector<std::shared_ptr<dag::DagNode>> startNodes;
    build_dag(&dependencies, &startNodes);

    for (auto startNode: startNodes) {
        std::set<size_t> accumulator;
        count_nodes(startNode, accumulator);
        std::cout << "Dag has " << accumulator.size() << " nodes." << std::endl;

        print_nodes(startNode, 0);
    }

    return EXIT_SUCCESS;
} 
