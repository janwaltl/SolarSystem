#ifndef SIMMETHODS_SIMMETHOD_HEADER
#define SIMMETHODS_SIMMETHOD_HEADER

#include "../Common/SystemUnit.h"

namespace solar
{
	//Methods that is used to simulate the data
	class SimMethod : public SystemUnit
	{
	public:
		//Operates on data, makes one physical time step
		virtual void operator()(simData_t& data, double step) = 0;
		virtual ~SimMethod() = default;
	};
}

#endif