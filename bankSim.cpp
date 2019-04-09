//
// Created by William Marringa on 2019-03-31.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "bankSim.h"
#include "Event.hpp"

bankSim::bankSim(double arrivalRate, double maxServiceTime, int seed) {
    currentTime = 0;
    this->newCustomerArrivalRate = arrivalRate;
    double secPerMin = 60.0;
    this->maxCustomerServiceTime = static_cast<int>(maxServiceTime * secPerMin);
    this->seed = seed;
}

bool bankSim::isTellerAvailable() {
    for (bool teller : isTellersServicing) {
        if (!teller) {
            return true;
        }
    }
    return false;
}

void bankSim::serveCustomer(Event customer) {
    for (bool &teller : isTellersServicing) {
        if (!teller) {
            teller = true;
            customer.beingServed = 1;
            customer.totalServiceTime = customer.arrivalTime;
            double secPerMin = 60.0;
            customer.firingTime = (currentTime/secPerMin) + customer.serviceTime;
            eventTiming.push(customer);
            break;
        }
    }
}

void bankSim::completeService(Event customer) {
    for (bool &teller: isTellersServicing) {
        if (teller) {
            teller = false;
            customer.totalServiceTime = customer.firingTime - customer.arrivalTime;
            allCustomersWaitingTimes.push_back(customer.totalServiceTime);
            if (!line.empty()) {
                Event nextCustomer = line.front();
                serveCustomer(nextCustomer);
                line.pop();
            }
            break;
        }
    }
}

void bankSim::printResult(){
    std::sort(allCustomersWaitingTimes.begin(), allCustomersWaitingTimes.end());
    double index10Tail = .10 * allCustomersWaitingTimes.size();
    double index50Tail = .50 * allCustomersWaitingTimes.size();
    double index90Tail = .90 * allCustomersWaitingTimes.size();
    std::cout << "BANK\n";
    std::cout << "Total number of customers served: " << allCustomersWaitingTimes.size() << "\n";
    std::cout << "10th percentile: " << std::setprecision(3) << allCustomersWaitingTimes[index10Tail] << "\n";
    std::cout << "50th percentile: " << std::setprecision(3) << allCustomersWaitingTimes[index50Tail] << "\n";
    std::cout << "90th percentile: " << std::setprecision(4) << allCustomersWaitingTimes[index90Tail] << "\n";
}

void bankSim::newComeCustomer(){
    srand(static_cast<unsigned int>(seed));
    
    double secPerMin = 60.0;
    int arrivalSpacing = static_cast<int>((1 / newCustomerArrivalRate) * secPerMin);
    
    double hr = 12.0;
    double hrPerMin = 60.0;
    int simTime = hr * hrPerMin * secPerMin;
    for (int s = arrivalSpacing; s < simTime; s += arrivalSpacing) {
        int minCustomerServiceTime = 2;
        Event customer = Event(s / secPerMin, (rand() % maxCustomerServiceTime + minCustomerServiceTime) / secPerMin);
        eventTiming.push(customer);
    }
}

//only one waiting line for six tellers
void bankSim::run() {
    
    newComeCustomer();
    
    while (!eventTiming.empty()) {
        Event c = eventTiming.top();
        eventTiming.pop();
        double secPerMin = 60.0;
        currentTime = (c.firingTime * secPerMin);
        int qsize = line.size();
        if (c.beingServed == 0) {
            if (isTellerAvailable() && line.empty()) {
                serveCustomer(c);
            } else {
                line.push(c);
            }
        } else {
            completeService(c);
        }
    }
    printResult();
    
}


