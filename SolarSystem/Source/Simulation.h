#ifndef SIMULATION_HEADER
#define SIMULATION_HEADER

#include <chrono>
#include "Common/Timing.h"
#include "Units/Unit.h"
#include "Units/SystemUnitTypes.h"

namespace solar
{

	//Simulates solar system
	class Simulation
	{
	public:
		Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer);
		//Starts simulation - loops until stopped, or maxRunTime is reached
		//Throws Exception(std::logic_error) on invalid input or if any other SystemUnit throws.
		//dTime - how often simMethod is called 
		//rawMult - how many times is simMethod called for each dTime
		//DTMult - how many dTimes are passed to simMethod.
		//example: dTime=1ms; rawMult=20;DTMult=5
		//			 Means that each 1ms simMethod will be called 20times with 5ms being passed as dTime to it
		void Start(stepTime_t dt, size_t rawMult = 1, size_t DTMult = 1,
				   std::chrono::seconds maxRunTime = std::chrono::seconds::zero());
		//Starts simulation - loops until stopped, or maxRunTime is reached.
		//Throws Exception(std::logic_error) on invalid input or if any other SystemUnit throws.
		//dTime - how often simMethod is called
		//rawMult - how many times is simMethod called for each viewer call.
		void StartNotTimed(stepTime_t dt, size_t rawMult = 1,
						   std::chrono::seconds maxRunTime = std::chrono::seconds::zero());
		//Ends the simulation
		void StopSimulation();
		//Pauses simMethod, only calls viewer
		void PauseSimulation();
		//Resumes paused simulation
		void ResumeSimulation();
		//Makes one step of simulation, then pauses again
		//Does NOT work in NotTimed Start
		void StepSimulation();
		bool IsPaused();
		bool IsRunnig();
		//Returns deltaTime in seconds
		double GetDtime() const;
		void SetDTime(double newDT);
		//Returns elapsed realTime in seconds
		double GetRunTime() const;
		//Returns elapsed simTime in seconds
		const simulatedTime& GetSimTime() const;
		//Sets new simTime in seconds
		void SetSimTime(simulatedTime newSimTime);
		//Returns last's frame time
		//Does NOT work in NotTimed Start
		double GetFrameTime() const;
		size_t GetRawMultiplier() const;
		size_t GetDTMultiplier() const;
		void SetRawMultiplier(size_t newRawMult);
		void SetDTMultiplier(size_t newDTMult);
	private:
		enum simState
		{
			notRunning,
			paused,
			running,
			stepping,
		};
		void Loop();
		void LoopNotTimed();
		//Clear timers before start of simulation's loop
		void ResetTimers();
		//Updates time in the loop
		void TickTime();
		void UpdateSimTime();
		bool IsNotRunningForTooLong();

		parser_p parser;
		simMethod_p simMethod;
		viewer_p viewer;

		//Start: Controls speed of simulation, for each DT pass, simMethod will get called this times
		//StartNotTimed: how many times is simMethod called for each viewer call
		size_t rawMultiplier;
		//Controls speed of simulation, passes this*dTime to simMethod.
		size_t DTMultiplier;
		//Size of step
		stepTime_t dtime;
		//Time bank, created by real time, consumed by physics simulation
		stepTime_t acc;
		//Last frame's time
		stepTime_t frameTime;

		//Amount of simulated time
		simulatedTime simTime;
		//How long has been the simulation running in real time
		stepTime_t runTime;
		//How long can the simulation run in real time
		stepTime_t maxRunTime;
		//Helper variables to compute frameTime
		clock_t::time_point prevTime, begining;

		//Data to be simulated
		//Pointed to by viewer and simMethod
		simData_t data;
		//Whether the simulation is running or not
		simState state;
		//If sim was unpaused last frame
		bool justUnpaused;
	};
}


#endif