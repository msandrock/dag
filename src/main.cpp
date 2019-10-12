#include <iostream> // std::cout
#include <string>
#include <vector>
#include "stdafx.hpp"

void verify_format(const std::string& line) {
    // Make sure there is exactly one '>'
    auto pos = line.find('>');

    if (pos == std::string::npos) {
        throw Exception("The line does not contain a '>'");
    }

    // Make sure there are no additional '>'
    if (line.find('>', pos + 1) != std::string::npos) {
        throw Exception("The line must contain exactly one '>'");
    }
}

int main(int argc, const char** argv) {
    // Collect parsed lines
    std::vector<std::string> lines;

    auto lineCount = 1;
    for (std::string line; std::getline(std::cin, line); lineCount++) {
        trim(line);

        try {
            verify_format(line);
            lines.push_back(line);
        } catch(Exception e) {
            std::cout << "Could not parse line " << lineCount << " \"" << line << "\": " << e.getMessage() << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Print all parsed lines
    for (auto line: lines) {
        std::cout << line << std::endl;
    }
 
    return EXIT_SUCCESS;
} 
