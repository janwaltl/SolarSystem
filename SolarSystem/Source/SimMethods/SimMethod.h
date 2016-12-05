#ifndef SIMMETHODS_SIMMETHOD_HEADER
#define SIMMETHODS_SIMMETHOD_HEADER

#include "../Common/Child.h"

//Methods that is used to simulate the data
class SimMethod : public Child
{
public:
	SimMethod();

	//Operates on elements, makes one physical time step
	virtual void operator()(elements_t& elements, double step) = 0;
};

#endif