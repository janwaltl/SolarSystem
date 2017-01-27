#ifndef VIEWERS_VIEWER_HEADER
#define VIEWERS_VIEWER_HEADER


#include <cassert>
#include "Source/Units/SystemUnit.h"
#include "Source/Units/Unit.h"

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
		friend void LinkUnitAndLinkedUnit(Viewer& linkedUnit, Viewer& toBeLinkedUnit);
	protected:
		// Pointer to simulated data, valid in operator() and Prepare().
		// IT IS NOT VALID IN constructor of derived classed
		simData_t* data {nullptr};
	};
	//Links SimMethod and simulation together, so it has access to simData
	inline void LinkUnitAndSim(Viewer& unit, Simulation& sim, simData_t* data)
	{
		assert(data);
		LinkUnitAndSim(static_cast<SystemUnit&>(unit), sim);
		unit.data = data;
	}
	//Links two viewers( used in nested viewers, for now in ViewAndRecord)
	inline void LinkUnitAndLinkedUnit(Viewer& linkedUnit, Viewer& toBeLinkedUnit)
	{
		LinkUnitAndLinkedUnit(static_cast<SystemUnit&>(linkedUnit), static_cast<SystemUnit&>(toBeLinkedUnit));
		toBeLinkedUnit.data = linkedUnit.data;
	}

}

#endif