#pragma once
#include <algorithm> 
#include <cctype>
#include <exception>    // std::exception
#include <locale>
#include <string>       // std::string
#include <sstream>

//
// Exceptions
//
class Exception : public std::exception {
    protected:
    std::string message;

    public:
    Exception(std::string message)
        : message(message) {
    }

    std::string getMessage() {
        return this->message;
    }
};

class ParseException : public Exception {
    std::string line;
    int lineNumber;

    public:
    ParseException(std::string message, std::string line, int lineNumber)
        : Exception(message), line(line), lineNumber(lineNumber) {
    }

    std::string getMessage() {
        std::stringstream message;
        message << "Could not parse line " << this->lineNumber << " \"" << this->line << "\": " << this->message; 
        return message.str(); 
    }
};

//
// Trim functions
//

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}
