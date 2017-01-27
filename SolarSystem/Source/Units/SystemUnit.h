#ifndef COMMON_SYSTEM_UNIT_HEADER
#define COMMON_SYSTEM_UNIT_HEADER

#include "Source/Common/Timing.h"

namespace solar
{
	class SystemUnit;
	class Simulation;
	void LinkUnitAndSim(SystemUnit& unit, Simulation& sim);

	// Base class for all modules - Parsers, SimMethods, Viewers
	// So they can be linked to simulation and can control it
	// Also indirects control of simulation
	class SystemUnit
	{
	public:
		void StopSimulation();
		void PauseSimulation();
		void ResumeSimulation();
		//Simulates one step of the simulation, then pauses again
		void StepSimulation();
		bool IsRunning();
		bool IsPaused();
		//Returns deltaTime in seconds
		double GetDtime() const;
		//Sets new delta time in seconds
		void SetDTime(double newDT);
		//Returns elapsed realTime in seconds
		double GetRunTime() const;
		//Returns precise simulated time
		simulatedTime GetSimTime() const;
		//Returns simulated time in seconds
		double GetSimTimeSecs() const;
		//Sets new sim time
		void SetSimTime(simulatedTime newSimTime);
		//Returns last's frame time in seconds
		double GetFrameTime() const;

		size_t GetDTMultiplier() const;
		size_t GetRawMultiplier() const;

		void SetRawMultiplier(size_t newRawMult);
		void SetDTMultiplier(size_t newDTMult);
	private:
		friend void LinkUnitAndSim(SystemUnit& unit, Simulation& sim);
		friend void LinkUnitAndLinkedUnit(SystemUnit& linkedUnit, SystemUnit& toBeLinkedUnit);

		Simulation* sim {nullptr};
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