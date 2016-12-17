#include "SystemUnit.h"
#include "../Simulation.h"

namespace solar
{

	void SystemUnit::StopSimulation()
	{
		assert(sim);
		sim->StopSimulation();
	}
	double SystemUnit::GetDtime()
	{
		assert(sim);
		return sim->GetDtime();
	}
}