#ifndef SIMMETHODS_SIMMETHOD_HEADER
#define SIMMETHODS_SIMMETHOD_HEADER

#include <cassert>
#include "Source/Units/SystemUnit.h"
#include "Source/Units/Unit.h"

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
		virtual void Prepare() {};
		virtual ~SimMethod() = default;

		friend void LinkUnitAndSim(SimMethod& unit, Simulation& sim, SimData* data);
	protected:
		// Pointer to simulated data, valid in operator() and Prepare().
		// IT IS NOT VALID IN constructor of derived classed
		SimData* data {nullptr};
	};
	//Links SimMethod and simulation together, so it has access to simData
	inline void LinkUnitAndSim(SimMethod& unit, Simulation& sim, SimData* data)
	{
		assert(data);
		LinkUnitAndSim(static_cast<SystemUnit&>(unit), sim);
		unit.data = data;
	}
}

#endif