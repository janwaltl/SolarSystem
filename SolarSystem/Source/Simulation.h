#ifndef SIMULATION_HEADER
#define SIMULATION_HEADER

#include <chrono>
#include "Common/Common.h"

//Simulates solar system
class Simulation
{
public:
	using clock_t = std::chrono::high_resolution_clock;
	using stepTime_t = clock_t::duration;
	Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer);
	//Starts simulation - loops until stopped, or maxSimulationTime is reached
	void Start(stepTime_t dt, std::chrono::seconds maxSimTime = std::chrono::seconds::zero());
	//Breaks the loop, called internally by parsers,view
	void StopSimulation();

private:
	void Loop();
	void ResetTimers();
	stepTime_t TickTime();
	bool IsNotRunningForTooLong();
	parser_p parser;
	simMethod_p simMethod;
	viewer_p viewer;

	//Size of step
	stepTime_t dtime;
	//Time bank, created by real time, consumed by physics simulation
	stepTime_t acc;
	//Amount of simulated time
	stepTime_t simTime;
	//How long has been the simulation running in real time
	stepTime_t runTime;
	//How long can the simulation run in real time
	stepTime_t maxSimTime;;
	//Helper variables to compute frameTime
	clock_t::time_point prevTime,begining;

	//Data to be simulated
	simData_t data;
	//Whether the simulation is running or not
	bool running;
};


#endif