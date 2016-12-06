#ifndef VIEWERS_VIEWER_HEADER
#define VIEWERS_VIEWER_HEADER

#include "../Common/SystemUnit.h"

// Hooked up to simulation, gets access each frame to simulated data
class Viewer :public SystemUnit
{
public:
	Viewer();
	virtual void operator()(elements_t& elements) = 0;
};

#endif