#include "QueueManager.h"

QueueManager::QueueManager(int t, int h, int d, DungeonManager& manager)
    : tanks(t), healers(h), dps(d), dungeonManager(manager) {
}

void QueueManager::processQueue() {
    int instanceID = 1;

    while (tanks >= 1 && healers >= 1 && dps >= 3 && dungeonManager.canStartInstance()) {
        tanks--;
        healers--;
        dps -= 3;

        dungeonManager.startDungeonInstance(instanceID++);
    }

    std::cout << "\nLeftover Players:\n";
    std::cout << "Tanks: " << tanks << ", Healers: " << healers << ", DPS: " << dps << "\n";
}