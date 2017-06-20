#ifndef ELEMENTS_ELEMENT_HEADER
#define ELEMENTS_ELEMENT_HEADER

#include <string>
#include <vector>
#include "Source/Math/Math.h"

namespace solar
{
	//Basic unit of simulated data - planet, star, moon...
	class Unit
	{
	public:
		explicit Unit(const Vec2d& pos = Vec2d(), const Vec2d&vel = Vec2d(), double mass = 0.0, const std::string& name = "",
					  const Vec4d& color = Vec4d(1.0, 1.0, 1.0, 1.0))
			:pos(pos), vel(vel), mass(mass), name(name), color(color)
		{}

		Vec2d pos, vel;
		double mass;
		Vec4d color;
		std::string name;
	};

	//Vector holding all simulated units together
	using simData_t = std::vector<Unit>;
}


#endif