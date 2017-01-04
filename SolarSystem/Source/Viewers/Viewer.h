#ifndef VIEWERS_VIEWER_HEADER
#define VIEWERS_VIEWER_HEADER

#include "../Common/SystemUnit.h"

namespace solar
{
	// Hooked up to simulation, gets access each frame to simulated data
	class Viewer :public SystemUnit
	{
	public:

		//View data, called each frame
		virtual void operator()() = 0;
		//Gives access to freshly loaded data before simulation starts
		virtual void Prepare() {}
		virtual ~Viewer() = default;

		friend void LinkUnitAndSim(Viewer& unit, Simulation& sim, simData_t* data);
	protected:
		// Pointer to simulated data, valid in operator() and Prepare().
		// IT IS NOT SET IN constructor of derived classed
		simData_t* data;
	};
	//Links SimMethod and simulation together, so it has access to simData
	inline void LinkUnitAndSim(Viewer& unit, Simulation& sim, simData_t* data)
	{
		assert(simData);
		LinkUnitAndSim(static_cast<SystemUnit&>(unit), sim);
		unit.data = data;
	}

}

#endif