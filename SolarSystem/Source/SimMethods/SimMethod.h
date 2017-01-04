#ifndef SIMMETHODS_SIMMETHOD_HEADER
#define SIMMETHODS_SIMMETHOD_HEADER

#include "../Common/SystemUnit.h"

namespace solar
{
	//Method that is used to simulate the data
	class SimMethod : public SystemUnit
	{
	public:
		//Operates on data, makes one physical time step
		//step is in seconds
		virtual void operator()(double step) = 0;
		//Method for preparation, called once before start of simulation
		// - used for creating temp object, or initializing of state which depends on for eg. size of data
		virtual void Prepare()=0;
		virtual ~SimMethod() = default;
	
		//Captures data, called only from Simulation itself
		void _Prepare(simData_t* simData)
		{
			assert(simData);
			this->data = simData;
			this->Prepare();
		}
	protected:
		// Pointer to simulated data, valid in operator() and Prepare().
		// IT IS NOT SET IN constructor of derived classed
		simData_t* data;
	};
}

#endif