//
// Created by William Marringa on 2019-04-01.
//

#ifndef SIMULATOR_EVENT_H
#define SIMULATOR_EVENT_H

struct Event {
    int beingServed;
    int groceryLine;
    double arrivalTime; // minutes
    double serviceTime; // minutes
    double totalServiceTime;
    double firingTime;

    Event(double arrivalTime, double serviceTime) {
        this->arrivalTime = arrivalTime;
        this->serviceTime = serviceTime;
        this->beingServed = 0;
        this->totalServiceTime = 0;
        this->firingTime = arrivalTime;
    }
};

struct CompareEvent {
    bool operator()(Event const &e1, Event const &e2) {
        return e1.firingTime > e2.firingTime;
    }
};
#endif //SIMULATOR_EVENT_H
