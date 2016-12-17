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
		//Get DeltaTime of simulation in seconds
		double GetDtime();
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