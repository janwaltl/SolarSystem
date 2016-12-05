#ifndef COMMON_CHILD_HEADER
#define COMMON_CHILD_HEADER

#include "Common.h"

//Base class for all components - Parsers,SimMethods,Viewers
//So they can be linked to simulation and can control it
class Child
{
public:
	void Link(Simulation* s);
	void StopSimulation();
private:
	Simulation* sim {};
};

#endif