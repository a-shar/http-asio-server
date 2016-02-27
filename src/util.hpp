#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <regex>

std::vector<std::string> split(const std::string& input) {
    std::regex re(" ");
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

#endif // UTIL_H
