#include "QueueManager.h"
#include <iostream>

int main() {
    try {
        std::cout << "Dungeon Queue Management System" << std::endl;
        std::cout << "======================================" << std::endl;

        // Get user inputs
        int n, t, h, d;
        double t1, t2;

        std::cout << "Maximum number of concurrent instances (n): ";
        std::cin >> n;

        std::cout << "Number of tank players in queue: ";
        std::cin >> t;

        std::cout << "Number of healer players in queue: ";
        std::cin >> h;

        std::cout << "Number of DPS players in queue: ";
        std::cin >> d;

        std::cout << "Minimum instance clear time (t1 seconds): ";
        std::cin >> t1;

        std::cout << "Maximum instance clear time (t2 seconds, <= 15): ";
        std::cin >> t2;

        // Validate inputs
        if (n <= 0 || t < 0 || h < 0 || d < 0 || t1 < 0 || t2 < t1 || t2 > 15) {
            std::cout << "Invalid input values. Please try again." << std::endl;
            return 1;
        }

        // Create and start the queue manager

        QueueManager manager(n, t, h, d, t1, t2);
        manager.start();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}