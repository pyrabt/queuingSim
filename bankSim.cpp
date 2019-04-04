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
    this->arrivalRate = arrivalRate;
    this->maxCustomerServiceTime = static_cast<int>(maxServiceTime * 60);
    this->seed = seed;
}

bool bankSim::isTellerAvailable() {
    for (bool teller : tellers) {
        if (!teller) {
            return true;
        }
    }
    return false;
}

void bankSim::serveCustomer(Event customer) {
    for (bool &teller : tellers) {
        if (!teller) {
            teller = true;
            customer.beingServed = 1;
            customer.totalServiceTime = customer.arrivalTime;
            customer.firingTime = (currentTime/60.0) + customer.serviceTime;
            eventTiming.push(customer);
            break;
        }
    }
}

void bankSim::completeService(Event customer) {
    for (bool &teller: tellers) {
        if (teller) {
            teller = false;
            customer.totalServiceTime = customer.firingTime - customer.arrivalTime;
            waitingTimes.push_back(customer.totalServiceTime);
            if (!line.empty()) {
                Event nextCustomer = line.front();
                serveCustomer(nextCustomer);
                line.pop();
            }
            break;
        }
    }
}

void bankSim::run() {

    srand(static_cast<unsigned int>(seed));

    int arrivalSpacing = static_cast<int>((1 / arrivalRate) * 60);

    for (int s = arrivalSpacing; s < 43200; s += arrivalSpacing) {
        Event customer = Event(s / 60.0, (rand() % maxCustomerServiceTime + 2) / 60.0);
        eventTiming.push(customer);
    }

    while (!eventTiming.empty()) {
        Event c = eventTiming.top();
        eventTiming.pop();
        currentTime = (c.firingTime * 60);
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

    std::sort(waitingTimes.begin(), waitingTimes.end());
    double index10 = .10 * waitingTimes.size();
    double index50 = .50 * waitingTimes.size();
    double index90 = .90 * waitingTimes.size();
    std::cout << "BANK\n";
    std::cout << "Total number of customers served: " << waitingTimes.size() << "\n";
    std::cout << "10th percentile: " << std::setprecision(3) << waitingTimes[index10] << "\n";
    std::cout << "50th percentile: " << std::setprecision(3) << waitingTimes[index50] << "\n";
    std::cout << "90th percentile: " << std::setprecision(4) << waitingTimes[index90] << "\n";
}


