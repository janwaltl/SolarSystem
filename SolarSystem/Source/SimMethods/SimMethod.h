#ifndef SIMMETHODS_SIMMETHOD_HEADER
#define SIMMETHODS_SIMMETHOD_HEADER

#include "../Common/SystemUnit.h"

//Methods that is used to simulate the data
class SimMethod : public SystemUnit
{
public:
	//Operates on elements, makes one physical time step
	virtual void operator()(elements_t& elements, double step) = 0;
};

#endif