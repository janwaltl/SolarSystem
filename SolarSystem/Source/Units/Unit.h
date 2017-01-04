#ifndef ELEMENTS_ELEMENT_HEADER
#define ELEMENTS_ELEMENT_HEADER

#include "../Math/Math.h"
#include <string>
namespace solar
{
	//Basic unit of simulated data - planet, star, moon...
	class Unit
	{
	public:
		explicit Unit(const Vec2& pos=Vec2(), const Vec2&vel=Vec2(), double mass=0.0) :pos(pos), vel(vel), mass(mass) {}
		Vec2 pos, vel;
		double mass;
		Vec4 color;
		std::string name;
	};
}


#endif