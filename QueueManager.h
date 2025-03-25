#pragma once
#include "DungeonInstance.h"
#include "Party.h"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

// Class to manage the dungeon queue
class QueueManager {
private:
    int max_instances;
    double t1, t2;
    std::queue<int> tank_queue;
    std::queue<int> healer_queue;
    std::queue<int> dps_queue;
    std::vector<std::unique_ptr<DungeonInstance>> instances;  // FIXED: Store as unique_ptr
    std::mutex queue_mutex;
    std::mutex console_mutex;
    std::atomic<bool> all_parties_formed;
    std::atomic<bool> shutdown_requested;
    std::vector<std::thread> active_threads;
    std::condition_variable cv;

public:
    QueueManager(int n, int tanks, int healers, int dps, double min_time, double max_time);
    ~QueueManager();

    // Try to form a party
    std::unique_ptr<Party> formParty();

    // Find an available instance
    DungeonInstance* findAvailableInstance();  // FIXED: Returns a pointer

    // Run a party in an instance
    void runParty(std::unique_ptr<Party> party);

    // Monitor the queues and form parties
    void queueMonitor();

    // Display the current status of all instances
    void displayStatus();

    // Display final statistics
    void displayFinalStats();

    // Check if any instances are active
    bool anyActiveInstances();

    // Start the queue management system
    void start();
};



