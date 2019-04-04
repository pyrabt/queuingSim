//
// Created by William Marringa on 2019-03-31.
//

#ifndef SIMULATOR_GROCERYSIM_H
#define SIMULATOR_GROCERYSIM_H


#include <array>
#include <queue>
#include "Event.hpp"

struct Cashier {
    bool helpingCustomer;
    double totalWait;
    std::queue<Event> line;
    Cashier() {
        helpingCustomer = false;
        totalWait = 0;
    }
};

class grocerySim {
private:
    //std::array<bool, 6> cashiers = {false, false, false, false, false, false};
    std::array<Cashier, 6> cashiers = {Cashier(), Cashier(), Cashier(), Cashier(), Cashier(), Cashier() };
    //std::array<std::queue<Event>, 6> lines;
    std::priority_queue <Event, std::vector<Event>, CompareEvent> eventTiming;
    std::vector<double> waitingTimes;
    long currentTime; //seconds
    double arrivalRate; //std::stod(argv[1]);
    int maxCustomerServiceTime; //std::stod(argv[2]);
    int seed;

    int isCashierAvailable();

    void serveCustomer(Event &customer);

    void completeService(Event &customer);

    int shortestLine();

    void joinLine(int shortestLineID, Event &customer);

    void serveNextCustomer(int lineNumber);

public:
    grocerySim(double arrivalRate, double maxServiceTime, int seed);

    void run();
};


#endif //SIMULATOR_GROCERYSIM_H
