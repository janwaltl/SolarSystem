#ifndef ELEMENTS_ELEMENT_HEADER
#define ELEMENTS_ELEMENT_HEADER

#include "../Math/Math.h"
//Basic unit of simulated data - planet, star, moon...
class Unit
{
public:
	Vec2 pos, vel;
	double mass {};
};


#endif