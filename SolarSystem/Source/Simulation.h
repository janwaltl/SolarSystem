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
	//Throws Exception(std::logic_error) on invalid input
	void Start(stepTime_t dt, std::chrono::seconds maxSimTime = std::chrono::seconds::zero());
	//Breaks the loop, called internally by parsers,view
	void StopSimulation();

private:
	void Loop();
	//Clear timers before start of simulation's loop
	void ResetTimers();
	//Updates time in the loop
	stepTime_t TickTime();
	bool IsNotRunningForTooLong();
	template<typename Rep,typename Per>
	//Converts passed time duration to seconds
	inline double ToSecs(const std::chrono::duration<Rep, Per>& time)
	{
		using type = std::chrono::duration<Rep, Per>;
		return  time.count() * type::period::num / double(type::period::den);
	}
	parser_p parser;
	simMethod_p simMethod;
	viewer_p viewer;

	//Size of step
	stepTime_t dtime;
	//Time bank, created by real time, consumed by physics simulation
	stepTime_t acc;
	//Amount of simulated time
	stepTime_t simTime;///Can actually count only to 270 years, which might be a problem, might need another in years...
	//How long has been the simulation running in real time
	stepTime_t runTime;
	//How long can the simulation run in real time
	stepTime_t maxSimTime;///With step time being in nanosecs and represented by long long, max is at around 270 years...
	//Helper variables to compute frameTime
	clock_t::time_point prevTime,begining;

	//Data to be simulated
	simData_t data;
	//Whether the simulation is running or not
	bool running;
};


#endif