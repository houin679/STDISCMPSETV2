#pragma once
#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "Utils.h"

class DungeonManager {
private:
    int maxInstances;
    int activeInstances;
    int partiesServed;
    int totalTimeServed;
    std::mutex mtx;
    std::condition_variable cv;

public:
    DungeonManager(int maxInst);
    void startDungeonInstance(int instanceID);
    void runDungeon(int instanceID);
    void printSummary();
    bool canStartInstance();
    void notifyInstanceComplete();
};

#endif