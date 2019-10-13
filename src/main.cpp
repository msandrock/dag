#include <iostream> // std::cout
#include <string>
#include <vector>
#include "stdafx.hpp"
#include "dag.hpp"

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
    std::hash<std::string> hash_fn;

    for (auto line: lines) {
        // Convert to dependency struct
        // lowercase and hash
        //std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
        const size_t pos = line.find('>');
        dag::Dependency dependency;

        if (pos != std::string::npos) {
            // Item has a child dependency
            dependency.ancestor = line.substr(0, pos);
            dependency.child = line.substr(pos + 1);
            dependency.ancestor_hash = hash_fn(dependency.ancestor);
            dependency.child_hash = hash_fn(dependency.child);
        } else {
            // Item is a standalone item without dependency
            dependency.ancestor = line;
            dependency.ancestor_hash = hash_fn(dependency.ancestor);
            dependency.child = "";
            dependency.child_hash = 0;
        }

        dependencies.push_back(dependency);
    }

    return dependencies;
}

int main(int argc, const char** argv) {
    // Collect parsed lines
    std::vector<std::string> lines; 
    try {
        lines = parse_stdin();
    } catch(ParseException e) {
        std::cout << e.getMessage() << std::endl;
        return EXIT_FAILURE;
    }

    // Convert parsed lines to dependency structs
    std::vector<dag::Dependency> dependencies = convert_dependencies(lines);
    std::vector<dag::DagNode> startNodes = build_dag(dependencies);

    for (const dag::DagNode startNode: startNodes) {
        print_nodes(startNode, 0);
    }

    return EXIT_SUCCESS;
} 
