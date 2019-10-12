#pragma once
#import <string>

struct Dependency {
    size_t ancestor_hash;
    size_t child_hash;

    std::string ancestor;
    std::string child;
};
