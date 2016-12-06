#ifndef SIMULATION_HEADER
#define SIMULATION_HEADER

#include <chrono>
#include "Common/Common.h"

//Simulates solar system
class Simulation
{
public:
	using runTime_t = std::chrono::milliseconds;

	Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer);
	//Starts simulation - loops until stopped, or maxSimulationTime is reached
	void Start(double dt, std::chrono::seconds maxSimTime = std::chrono::seconds(0));
	//Breaks the loop, called internally by parsers,view
	void StopSimulation();

private:
	void Loop();

	parser_p parser;
	simMethod_p simMethod;
	viewer_p viewer;

	double dtime;
	double acc;
	runTime_t runTime, maxSimTime;
	std::chrono::time_point<std::chrono::steady_clock> prevTime;

	simData_t data;
	bool running;
};


#endif