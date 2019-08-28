#ifndef MISSION_ACTION_HPP
#define MISSION_ACTION_HPP 

#include <vector>

struct Actions
{
    Actions(std::vector<std::string> i) : instructions(i) {}
    std::vector<std::string> instructions;
};

#endif
