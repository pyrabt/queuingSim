simulator: main.cpp Event.hpp grocerySim.cpp bankSim.cpp
	clang++ -std=c++17 -fsanitize=address main.cpp grocerySim.cpp bankSim.cpp -o simulator 

