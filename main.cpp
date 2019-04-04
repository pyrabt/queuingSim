#include <iostream>
#include <queue>
#include <array>
#include <list>
#include "Event.hpp"
#include "bankSim.h"
#include "grocerySim.h"


int main(int argc, const char *argv[]) {
    double arrivalRate = std::stod(argv[1]);
    double maxCustomerServiceTime = std::stod(argv[2]);
    int seed = std::stoi(argv[3]);
    if (argc == 5) {
        std::string simVersion = argv[4];
        for (char i : simVersion) {
            std::tolower(i);
        }
        if (simVersion == "bank") {
            bankSim bankSimulation = bankSim(arrivalRate, maxCustomerServiceTime, seed);
            bankSimulation.run();
        } else if (simVersion == "supermarket") {
            grocerySim grocerySimulation = grocerySim(arrivalRate, maxCustomerServiceTime, seed);
            grocerySimulation.run();
        }
    } else {
        bankSim bankSimulation = bankSim(arrivalRate, maxCustomerServiceTime, seed);
        bankSimulation.run();
        grocerySim grocerySimulation = grocerySim(arrivalRate, maxCustomerServiceTime, seed);
        grocerySimulation.run();
    }
    return 0;
}