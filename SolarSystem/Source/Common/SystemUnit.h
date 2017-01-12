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
		//Simulates one step of the simulation, then pauses again
		void StepSimulation();
		bool IsRunning();
		bool IsPaused();
		//Returns deltaTime in seconds
		double GetDtime();
		void SetDTime(double newDT);
		//Returns elapsed realTime in seconds
		double GetRunTime();
		//Returns elapsed simTime in seconds
		double GetSimTime();
		//Returns last's frame time
		double GetFrameTime();

		size_t GetDTMultiplier();
		size_t GetRawMultiplier();

		void SetRawMultiplier(size_t newRawMult);
		void SetDTMultiplier(size_t newDTMult);
	private:
		friend void LinkUnitAndSim(SystemUnit& unit, Simulation& sim);
		friend void LinkUnitAndLinkedUnit(SystemUnit& linkedUnit, SystemUnit& toBeLinkedUnit);

		Simulation* sim {};
	};
	//Links SystemUnit and simulation together
	inline void LinkUnitAndSim(SystemUnit& unit, Simulation& sim)
	{
		unit.sim = &sim;
	}
	//Links toBeLinked unit to same simulation as linkedUnit is linked
	///(Currently here because of replaying(=nested viewers), and making sim accessible seemed too weird)
	inline void LinkUnitAndLinkedUnit(SystemUnit& linkedUnit, SystemUnit& toBeLinkedUnit)
	{
		toBeLinkedUnit.sim = linkedUnit.sim;
	}
}

#endif