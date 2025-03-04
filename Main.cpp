#include "DungeonManager.h"
#include "QueueManager.h"
#include "PlayerQueue.h"

int t1, t2;

int main() {
    int n, t, h, d;

    // User input
    std::cout << "Enter max concurrent instances (n): ";
    std::cin >> n;
    std::cout << "Enter number of tanks (t): ";
    std::cin >> t;
    std::cout << "Enter number of healers (h): ";
    std::cin >> h;
    std::cout << "Enter number of DPS (d): ";
    std::cin >> d;
    std::cout << "Enter min dungeon time (t1): ";
    std::cin >> t1;
    std::cout << "Enter max dungeon time (t2): ";
    std::cin >> t2;

    DungeonManager dungeonManager(n);
    QueueManager queueManager(t, h, d, dungeonManager);

    queueManager.processQueue();
    dungeonManager.printSummary();

    std::cout << "\nPress ENTER to exit...";
    std::cin.get();
    std::cin.get();

    return 0;
}