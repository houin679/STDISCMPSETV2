#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <random>

extern int t1, t2;

inline int getRandomDungeonTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(t1, t2);
    return dist(gen);
}

#endif