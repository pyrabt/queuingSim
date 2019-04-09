//
// Created by William Marringa on 2019-03-31.
//

#ifndef SIMULATOR_GROCERYSIM_H
#define SIMULATOR_GROCERYSIM_H


#include <array>
#include <queue>
#include "Event.hpp"

struct Cashier {
    bool isHelpingCustomer;
    double totalWait;
    std::queue<Event> line;
    Cashier() {
        isHelpingCustomer = false;
        totalWait = 0;
    }
};

class grocerySim {
private:
    //std::array<bool, 6> cashiers = {false, false, false, false, false, false};
    std::array<Cashier, 6> cashiers = {Cashier(), Cashier(), Cashier(), Cashier(), Cashier(), Cashier() };
    //std::array<std::queue<Event>, 6> lines;
    std::priority_queue <Event, std::vector<Event>, CompareEvent> eventTiming;
    std::vector<double> allCustomersWaitingTimes;
    long currentTime; //seconds
    double newCustomerArrivalRate; //std::stod(argv[1]);
    int maxCustomerServiceTime; //std::stod(argv[2]);
    int seed;
    
    int availableCashierIndex();
    
    void serveCustomer(Event &customer);
    
    void completeService(Event &customer);
    
    int shortestLine();
    
    void joinLine(int shortestLineID, Event &customer);
    
    void serveNextCustomer(int lineNumber);
    
    void printResult();
    
    void newComeCustomer();
    
public:
    grocerySim(double arrivalRate, double maxServiceTime, int seed);
    
    void run();
};


#endif //SIMULATOR_GROCERYSIM_H
