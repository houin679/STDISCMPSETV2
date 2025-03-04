#include "DungeonManager.h"

DungeonManager::DungeonManager(int maxInst) {
    maxInstances = maxInst;
    activeInstances = 0;
    partiesServed = 0;
    totalTimeServed = 0;
}

void DungeonManager::startDungeonInstance(int instanceID) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return activeInstances < maxInstances; });

    activeInstances++;
    std::thread(&DungeonManager::runDungeon, this, instanceID).detach();
}

void DungeonManager::runDungeon(int instanceID) {
    std::cout << "Instance " << instanceID << " is now ACTIVE\n";
    int clearTime = getRandomDungeonTime();

    std::this_thread::sleep_for(std::chrono::seconds(clearTime));

    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Instance " << instanceID << " finished in " << clearTime << " seconds.\n";

    activeInstances--;
    partiesServed++;
    totalTimeServed += clearTime;
    cv.notify_all();
}

void DungeonManager::printSummary() {
    std::cout << "\nSummary:\n";
    std::cout << "Total parties served: " << partiesServed << "\n";
    std::cout << "Total time served: " << totalTimeServed << " seconds\n";
}

bool DungeonManager::canStartInstance() {
    return activeInstances < maxInstances;
}

void DungeonManager::notifyInstanceComplete() {
    cv.notify_all();
}