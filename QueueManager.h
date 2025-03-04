#pragma once
#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include "DungeonManager.h"

class QueueManager {
private:
    int tanks, healers, dps;
    DungeonManager& dungeonManager;

public:
    QueueManager(int t, int h, int d, DungeonManager& manager);
    void processQueue();
};

#endif
