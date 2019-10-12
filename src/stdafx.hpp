#pragma once
#include <exception>    // std::exception
#include <string>       // std::string

//
// Base class of all exceptions
//
class Exception : public std::exception {
    std::string message;

    public:
    Exception(std::string message)
        : message(message) {
    }

    std::string getMessage() {
        return this->message;
    }
};