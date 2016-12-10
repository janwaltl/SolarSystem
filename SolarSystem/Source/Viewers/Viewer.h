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
		virtual void operator()(simData_t&) = 0;
		//Gives access to freshly loaded data before simulation starts
		virtual void Prepare(const simData_t&) {}
		virtual ~Viewer() = default;
	};
}

#endif