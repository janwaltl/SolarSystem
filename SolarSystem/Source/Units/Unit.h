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
		explicit Unit(const Vec2& pos = Vec2(), const Vec2&vel = Vec2(), double mass = 0.0, const std::string& name = "",
					  const Vec4& color = Vec4(1.0, 1.0, 1.0, 1.0))
			:pos(pos), vel(vel), mass(mass), name(name), color(color)
		{

		}
		Vec2 pos, vel;
		double mass;
		Vec4 color;
		std::string name;
	};

	using simData_t = std::vector<Unit>;
}


#endif