#include "QueueManager.h"
#include <iostream>
#include <iomanip>

QueueManager::QueueManager(int n, int tanks, int healers, int dps, double min_time, double max_time)
    : max_instances(n), t1(min_time), t2(max_time), all_parties_formed(false), shutdown_requested(false) {

    // Initialize player queues
    for (int i = 1; i <= tanks; ++i) {
        tank_queue.push(i);
    }
    for (int i = 1; i <= healers; ++i) {
        healer_queue.push(i);
    }
    for (int i = 1; i <= dps; ++i) {
        dps_queue.push(i);
    }

    // Create dungeon instances
    for (int i = 0; i < n; ++i) {
        instances.emplace_back(std::make_unique<DungeonInstance>(i + 1)); // FIXED
    }
}

QueueManager::~QueueManager() {
    shutdown_requested = true;
    cv.notify_all();
    for (auto& thread : active_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

std::unique_ptr<Party> QueueManager::formParty() {
    std::lock_guard<std::mutex> lock(queue_mutex);

    if (tank_queue.size() >= 1 && healer_queue.size() >= 1 && dps_queue.size() >= 3) {
        auto party = std::make_unique<Party>();
        party->tank = tank_queue.front();
        tank_queue.pop();

        party->healer = healer_queue.front();
        healer_queue.pop();

        for (int i = 0; i < 3; ++i) {
            party->dps.push_back(dps_queue.front());
            dps_queue.pop();
        }

        return party;
    }
    else {
        // Check if we can't form any more parties
        if (tank_queue.empty() || healer_queue.empty() || dps_queue.size() < 3) {
            all_parties_formed = true;
        }
        return nullptr;
    }
}

DungeonInstance* QueueManager::findAvailableInstance() {
    for (const auto& instance : instances) {  // FIXED: Using const auto&
        if (instance->getStatus() == "empty") {  // FIXED: Using -> for unique_ptr
            return instance.get();
        }
    }
    return nullptr;
}

void QueueManager::runParty(std::unique_ptr<Party> party) {
    DungeonInstance* instance = nullptr;
    while (!shutdown_requested) {
        instance = findAvailableInstance();
        if (instance) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (instance && !shutdown_requested) {
        instance->runInstance(t1, t2);
    }
}

void QueueManager::queueMonitor() {
    std::vector<std::thread> party_threads;

    while (!shutdown_requested) {
        auto party = formParty();
        if (party) {
            std::thread party_thread(&QueueManager::runParty, this, std::move(party));
            party_threads.push_back(std::move(party_thread));
        }
        else if (all_parties_formed) {
            bool any_active = anyActiveInstances();

            if (!any_active) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    for (auto& thread : party_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void QueueManager::displayStatus() {
    while (!shutdown_requested && (!all_parties_formed || anyActiveInstances())) {
        system("cls");

        {
            std::lock_guard<std::mutex> lock(console_mutex);
            std::cout << "Dungeon Queue Management System" << std::endl;
            std::cout << "======================================" << std::endl;

            std::cout << "\nInstance Status:" << std::endl;
            for (const auto& instance : instances) {  // FIXED: Using const auto&
                std::cout << "Instance " << instance->getId() << ": " << instance->getStatus() << std::endl;
            }

            {
                std::lock_guard<std::mutex> queue_lock(queue_mutex);
                std::cout << "\nQueues:" << std::endl;
                std::cout << "Tank queue: " << tank_queue.size() << " players" << std::endl;
                std::cout << "Healer queue: " << healer_queue.size() << " players" << std::endl;
                std::cout << "DPS queue: " << dps_queue.size() << " players" << std::endl;
            }

            std::cout << "\nPress Ctrl+C to exit" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    displayFinalStats();
}

void QueueManager::displayFinalStats() {
    system("cls");

    std::lock_guard<std::mutex> lock(console_mutex);
    std::cout << "Dungeon Queue Management - Final Statistics" << std::endl;
    std::cout << "================================================" << std::endl;

    for (const auto& instance : instances) {  // FIXED: Using const auto&
        auto stats = instance->getStats();   // FIXED: Using -> for unique_ptr
        std::cout << "Instance " << instance->getId() << ":" << std::endl;
        std::cout << "  Parties served: " << stats.first << std::endl;
        std::cout << "  Total time served: " << std::fixed << std::setprecision(2) << stats.second << " seconds" << std::endl;
    }

    {
        std::lock_guard<std::mutex> queue_lock(queue_mutex);
        std::cout << "\nRemaining players in queue:" << std::endl;
        std::cout << "Tanks: " << tank_queue.size() << std::endl;
        std::cout << "Healers: " << healer_queue.size() << std::endl;
        std::cout << "DPS: " << dps_queue.size() << std::endl;
    }
}

bool QueueManager::anyActiveInstances() {
    for (const auto& instance : instances) {  // FIXED: Using const auto&
        if (instance->getStatus() == "active") {
            return true;
        }
    }
    return false;
}

void QueueManager::start() {
    std::thread display_thread(&QueueManager::displayStatus, this);
    std::thread monitor_thread(&QueueManager::queueMonitor, this);

    monitor_thread.join();
    shutdown_requested = true;

    if (display_thread.joinable()) {
        display_thread.join();
    }
}
