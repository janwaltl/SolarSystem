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
	public:
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