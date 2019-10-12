#include <iostream> // std::cout
#include <string>
#include <vector>
#include "stdafx.hpp"

void verify_format(const std::string& line, int lineNumber) {
    // Make sure there is exactly one '>'
    auto pos = line.find('>');

    if (pos == std::string::npos) {
        throw ParseException("The line does not contain a '>'", line, lineNumber);
    }

    // Make sure there are no additional '>'
    if (line.find('>', pos + 1) != std::string::npos) {
        throw ParseException("The line must contain exactly one '>'", line, lineNumber);
    }
}

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

int main(int argc, const char** argv) {
    // Collect parsed lines
    std::vector<std::string> lines; 
    try {
        lines = parse_stdin();
    } catch(ParseException e) {
        std::cout << e.getMessage() << std::endl;
        return EXIT_FAILURE;
    }


    // Print all parsed lines
    for (auto line: lines) {
        std::cout << line << std::endl;
    }
 
    return EXIT_SUCCESS;
} 
