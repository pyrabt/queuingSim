//
// Created by William Marringa on 2019-03-31.
//

#ifndef SIMULATOR_BANKSIM_H
#define SIMULATOR_BANKSIM_H


#include <array>
#include <queue>
#include "Event.hpp"

class bankSim {
private:
    std::array<bool, 6> tellers = {false, false, false, false, false, false};
    std::queue<Event> line;
    std::priority_queue <Event, std::vector<Event>, CompareEvent> eventTiming;
    std::vector<double> waitingTimes;
    long currentTime; //seconds
    double arrivalRate; //std::stod(argv[1]);
    int maxCustomerServiceTime; //std::stod(argv[2]);
    int seed;

    bool isTellerAvailable();

    void serveCustomer(Event customer);

    void completeService(Event customer);

public:
    bankSim(double arrivalRate, double maxServiceTime, int seed);
    void run();
};


#endif //SIMULATOR_BANKSIM_H
