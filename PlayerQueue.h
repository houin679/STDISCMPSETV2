#pragma once
#ifndef PLAYERQUEUE_H
#define PLAYERQUEUE_H

#include <iostream>

class PlayerQueue {
private:
    int tanks, healers, dps;

public:
    PlayerQueue(int t, int h, int d);
    int getTanks();
    int getHealers();
    int getDPS();
};

#endif