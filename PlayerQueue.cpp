#include "PlayerQueue.h"

PlayerQueue::PlayerQueue(int t, int h, int d) : tanks(t), healers(d), dps(d) {}

int PlayerQueue::getTanks() { return tanks; }
int PlayerQueue::getHealers() { return healers; }
int PlayerQueue::getDPS() { return dps; }