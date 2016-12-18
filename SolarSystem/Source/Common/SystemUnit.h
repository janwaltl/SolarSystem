#ifndef COMMON_SYSTEMUNIT_HEADER
#define COMMON_SYSTEMUNIT_HEADER

#include "Common.h"

namespace solar
{
	class SystemUnit;
	void LinkUnitAndSim(SystemUnit& unit, Simulation& sim);

	//Base class for all components - Parsers,SimMethods,Viewers
	//So they can be linked to simulation and can control it
	class SystemUnit
	{
	public:
		void StopSimulation();
		//Pauses the simulation
		void PauseSimulation();
		//Resumes paused simulation
		void ResumeSimulation();
		bool IsRunning();
		bool IsPaused();
		//Returns deltaTime in seconds
		double GetDtime();
		//Returns elapsed realTime in seconds
		double GetRunTime();
		//Returns elapsed simTime in seconds
		double GetSimTime();
		//Returns last's frame time
		double GetFrameTime();

		size_t GetDTMultiplier();
		size_t GetRawMultiplier();
	private:
		friend void LinkUnitAndSim(SystemUnit& unit, Simulation& sim);

		Simulation* sim {};
	};
	//Links SystemUnit and simulation together
	inline void LinkUnitAndSim(SystemUnit& unit, Simulation& sim)
	{
		unit.sim = &sim;
	}
}

#endif