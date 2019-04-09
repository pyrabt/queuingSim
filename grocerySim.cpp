//
// Created by William Marringa on 2019-03-31.
//

#include <iostream>
#include <iomanip>
#include "grocerySim.h"

grocerySim::grocerySim(double arrivalRate, double maxServiceTime, int seed) {
    currentTime = 0;
    this->newCustomerArrivalRate = arrivalRate;
    double secPerMin = 60.0;
    this->maxCustomerServiceTime = static_cast<int>(maxServiceTime * secPerMin);
    this->seed = seed;
}

int grocerySim::availableCashierIndex() {
    for (int i = 0; i < cashiers.size(); ++i) {
        if (!cashiers[i].isHelpingCustomer) {
            return i;
        }
    }
    return -1;
}

void grocerySim::serveCustomer(Event &customer) {
    cashiers[customer.groceryLine].isHelpingCustomer = true;
    double secPerMin = 60.0;
    customer.firingTime = (currentTime/secPerMin) + customer.serviceTime;
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
    cashiers[lineNum].isHelpingCustomer = false;
    allCustomersWaitingTimes.push_back((customer.firingTime - customer.arrivalTime));
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

void grocerySim::printResult(){
    std::sort(allCustomersWaitingTimes.begin(), allCustomersWaitingTimes.end());
    double index10 = .10 * allCustomersWaitingTimes.size();
    double index50 = .50 * allCustomersWaitingTimes.size();
    double index90 = .90 * allCustomersWaitingTimes.size();
    std::cout << "GROCERY STORE\n";
    std::cout << "Total number of customers served: " << allCustomersWaitingTimes.size() << "\n";
    std::cout << "10th percentile: " << std::setprecision(3) << allCustomersWaitingTimes[index10] << "\n";
    std::cout << "50th percentile: " << std::setprecision(3) << allCustomersWaitingTimes[index50] << "\n";
    std::cout << "90th percentile: " << std::setprecision(4) << allCustomersWaitingTimes[index90] << "\n";
}

void grocerySim::newComeCustomer(){
    srand(static_cast<unsigned int>(seed));
    double secPerMin = 60.0;
    int arrivalSpacing = static_cast<int>((1 / newCustomerArrivalRate) * secPerMin);
    double hr = 12.0;
    double minPerHr = 60.0;
    int totalTime = hr * minPerHr * secPerMin;
    
    for (int s = arrivalSpacing; s < totalTime; s += arrivalSpacing) {
        int minCustomerServiceTime = 2;
        Event customer = Event(s / secPerMin, (rand() % maxCustomerServiceTime + minCustomerServiceTime) / secPerMin);
        eventTiming.push(customer);
    }
}

void grocerySim::run() {
    
    newComeCustomer();
    
    while (!eventTiming.empty()) {
        Event c = eventTiming.top();
        eventTiming.pop();
        currentTime = static_cast<long>(c.firingTime * 60);
        if (c.beingServed == 0) {
            int cashierNumber = availableCashierIndex();
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
    printResult();
    
}
