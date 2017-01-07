#ifndef SIMULATION_HEADER
#define SIMULATION_HEADER

#include <chrono>
#include "Common/Common.h"

namespace solar
{
	//Simulates solar system
	class Simulation
	{
	public:
		using clock_t = std::chrono::steady_clock;
		using stepTime_t = clock_t::duration;
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
		double GetDtime();
		//Returns elapsed realTime in seconds
		double GetRunTime();
		//Returns elapsed simTime in seconds
		//Does NOT work in NotTimed Start
		double GetSimTime();
		//Returns last's frame time
		//Does NOT work in NotTimed Start
		double GetFrameTime();
		size_t GetRawMultiplier();
		size_t GetDTMultiplier();
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

		template<typename Rep, typename Per>
		//Converts time duration to seconds
		inline double ToSecs(const std::chrono::duration<Rep, Per>& time)
		{
			using type = std::chrono::duration<Rep, Per>;
			return  time.count() * type::period::num / double(type::period::den);
		}

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
		stepTime_t simTimePrecise;//Store fraction of seconds 
		std::chrono::seconds simTimeSecs;//store whole seconds

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