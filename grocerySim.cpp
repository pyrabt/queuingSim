//
// Created by William Marringa on 2019-03-31.
//

#include <iostream>
#include <iomanip>
#include "grocerySim.h"

grocerySim::grocerySim(double arrivalRate, double maxServiceTime, int seed) {
    currentTime = 0;
    this->arrivalRate = arrivalRate;
    this->maxCustomerServiceTime = static_cast<int>(maxServiceTime * 60);
    this->seed = seed;
}

int grocerySim::isCashierAvailable() {
    for (int i = 0; i < cashiers.size(); ++i) {
        if (!cashiers[i].helpingCustomer) {
            return i;
        }
    }
    return -1;
}

void grocerySim::serveCustomer(Event &customer) {
    cashiers[customer.groceryLine].helpingCustomer = true;
    customer.firingTime = (currentTime/60.0) + customer.serviceTime;
    customer.beingServed = 1;
    eventTiming.push(customer);
}

void grocerySim::serveNextCustomer(int lineNumber) {
    Event nextCustomer = cashiers[lineNumber].line.front();
    cashiers[lineNumber].line.pop();
    serveCustomer(nextCustomer);
}

void grocerySim::completeService(Event &customer) {
    int lineNum = customer.groceryLine;
    cashiers[lineNum].helpingCustomer = false;
    waitingTimes.push_back((customer.firingTime - customer.arrivalTime));
    cashiers[lineNum].totalWait -= customer.serviceTime;
    if (!cashiers[lineNum].line.empty()) {
        serveNextCustomer(lineNum);
    }
}

int grocerySim::shortestLine() {
    int shortestLineID = 0;
    double shortestWait = cashiers[0].totalWait;
    for (int i = 1; i < cashiers.size(); ++i) {
        if (cashiers[i].totalWait < shortestWait) {
            shortestLineID = i;
            shortestWait = cashiers[i].totalWait;
        }
    }
    return shortestLineID;
}

void grocerySim::joinLine(int shortestLineID, Event &customer) {
    customer.groceryLine = shortestLineID;
    cashiers[shortestLineID].line.push(customer);
    cashiers[shortestLineID].totalWait += customer.serviceTime;
}

void grocerySim::run() {

    srand(static_cast<unsigned int>(seed));

    int arrivalSpacing = static_cast<int>((1 / arrivalRate) * 60);

    for (int s = arrivalSpacing; s < 43200; s += arrivalSpacing) {
        Event customer = Event(s / 60.0, (rand() % maxCustomerServiceTime + 2) / 60.0);
        eventTiming.push(customer);
    }

    while (!eventTiming.empty()) {
        Event c = eventTiming.top();
        eventTiming.pop();
        currentTime = static_cast<long>(c.firingTime * 60);
        if (c.beingServed == 0) {
            int cashierNumber = isCashierAvailable();
            if (cashierNumber >= 0 && cashiers[cashierNumber].line.empty()) {
                c.groceryLine = cashierNumber;
                serveCustomer(c);
            } else {
                int lineNum = shortestLine();
                joinLine(lineNum, c);
            }
        } else {
            completeService(c);
        }
    }

    std::sort(waitingTimes.begin(), waitingTimes.end());
    double index10 = .10 * waitingTimes.size();
    double index50 = .50 * waitingTimes.size();
    double index90 = .90 * waitingTimes.size();
    std::cout << "GROCERY STORE\n";
    std::cout << "Total number of customers served: " << waitingTimes.size() << "\n";
    std::cout << "10th percentile: " << std::setprecision(3) << waitingTimes[index10] << "\n";
    std::cout << "50th percentile: " << std::setprecision(3) << waitingTimes[index50] << "\n";
    std::cout << "90th percentile: " << std::setprecision(4) <<waitingTimes[index90] << "\n";
}
