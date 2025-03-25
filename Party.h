#pragma once

#include <vector>

// Structure to represent a party
struct Party {
    int tank = 0;
    int healer = 0;
    std::vector<int> dps;

    // Constructor to ensure explicit initialization
    Party() : tank(0), healer(0), dps(std::vector<int>()) {}
};
