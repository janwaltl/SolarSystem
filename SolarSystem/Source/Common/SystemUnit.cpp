#include "SystemUnit.h"
#include "../Simulation.h"

namespace solar
{
	void SystemUnit::Link(Simulation* s)
	{
		assert(s);
		sim = s;
	}
	void SystemUnit::StopSimulation()
	{
		assert(sim);
		sim->StopSimulation();
	}
}