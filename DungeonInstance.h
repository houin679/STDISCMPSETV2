#pragma once

#include <string>
#include <mutex>
#include <utility>

// Class to represent a dungeon instance
class DungeonInstance {
private:
    int instance_id;
    std::string status;
    int parties_served;
    double total_time_served;
    mutable std::mutex instance_mutex;  // FIX: Added 'mutable'

public:
    DungeonInstance(int id);

    // Run a dungeon instance for a random time between t1 and t2 seconds
    bool runInstance(double t1, double t2);

    // Get the current status of the instance
    std::string getStatus();

    // Get statistics about the instance
    std::pair<int, double> getStats() const;

    // Get the instance ID
    int getId() const;
};
