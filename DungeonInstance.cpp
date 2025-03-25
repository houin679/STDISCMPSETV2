#include "DungeonInstance.h"
#include <random>
#include <thread>
#include <chrono>

DungeonInstance::DungeonInstance(int id)
    : instance_id(id), status("empty"), parties_served(0), total_time_served(0) {
}

bool DungeonInstance::runInstance(double t1, double t2) {
    // Lock to update status
    {
        std::lock_guard<std::mutex> lock(instance_mutex);
        status = "active";
    }

    // Random time to complete the dungeon
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(t1, t2);
    double run_time = dist(gen);

    // Sleep for the run time
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(run_time * 1000)));

    // Update status and stats
    {
        std::lock_guard<std::mutex> lock(instance_mutex);
        status = "empty";
        parties_served++;
        total_time_served += run_time;
    }

    return true;
}

std::string DungeonInstance::getStatus() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    return status;
}


std::pair<int, double> DungeonInstance::getStats() const {
    std::lock_guard<std::mutex> lock(instance_mutex);
    return { parties_served, total_time_served };
}

int DungeonInstance::getId() const {
    return instance_id;
}