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
		Unit(const Vec2& pos, const Vec2&vel, double mass) :pos(pos), vel(vel), mass(mass) {}
		Unit() :Unit({0.0,0.0}, {0.0,0.0}, 0.0) {}
		Vec2 pos, vel;
		double mass;
		Vec4 color;
		std::string name;
	};
}


#endif