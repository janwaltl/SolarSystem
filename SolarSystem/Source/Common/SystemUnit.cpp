#include "SystemUnit.h"
#include "../Simulation.h"

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