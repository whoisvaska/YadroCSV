//
// Created by vaska on 16-Jan-23.
//

#include "string_helper.h"


std::vector<std::string> splitString(std::string input, char delimiter) {
    std::vector<std::string> values;
    std::string s;

    std::istringstream iss(input);
    while(std::getline(iss, s, delimiter))
    {
        values.push_back(s);
    }

    return values;
}