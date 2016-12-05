#include "Child.h"
#include "../Simulation.h"

void Child::Link(Simulation* s)
{
	assert(s);
	sim = s;
}
void Child::StopSimulation()
{
	assert(sim);
	sim->StopSimulation();
}