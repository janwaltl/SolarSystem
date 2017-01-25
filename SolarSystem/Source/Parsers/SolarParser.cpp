#include "SolarParser.h"

namespace solar
{
	simData_t SolarParser::Load()
	{
		simData_t data;
		//Sun
		data.push_back(Unit(Vec2(0.0, 0.0), Vec2(0.0, 0.0), 1.98843e30, "Sun", Vec4(1.0, 1.0, 0.0, 1.0)));
		//Planets
		data.push_back(Unit(Vec2(57.909e9, 0.0), Vec2(0.0, 47362.0), 3.3011e23, "Mercury", Vec4(0.2, 0.3, 0.2, 1.0)));
		data.push_back(Unit(Vec2(108.208e9, 0.0), Vec2(0.0, 35020.0), 3.3011e23, "Venus", Vec4(0.5, 0.2, 0.2, 1.0)));
		data.push_back(Unit(Vec2(149.6e9, 0.0), Vec2(0.0, 29800.0), 5.9736e24, "Earth", Vec4(0.17, 0.63, 0.83, 1.0)));
		data.push_back(Unit(Vec2(225.6e9, 0.0), Vec2(0.0, 24100.0), 6.4185e23, "Mars", Vec4(1.0, 0.46, 0.0, 1.0)));
		data.push_back(Unit(Vec2(778.5e9, 0.0), Vec2(0.0, 13050.0), 1.899e27, "Jupiter", Vec4(0.86, 0.76, 0.57, 1.0)));
		data.push_back(Unit(Vec2(1429e9, 0.0), Vec2(0.0, 9638.0), 5.6846e26, "Saturn", Vec4(1.0, 0.92, 0.78, 1.0)));
		data.push_back(Unit(Vec2(2870e9, 0.0), Vec2(0.0, 6795.0), 8.6832e25, "Uran", Vec4(0.71, 0.98, 0.96, 1.0)));
		data.push_back(Unit(Vec2(4498e9, 0.0), Vec2(0.0, 5432.0), 1.0243e26, "Neptun", Vec4(0.0, 0.3, 0.77, 1.0)));
		//Dwarf planets
		data.push_back(Unit(Vec2(7375.93e9, 0.0), Vec2(0.0, 3710.0), 1.303e22, "Pluto", Vec4(1.0, 1.0, 1.0, 1.0)));

		//Moons
		data.push_back(Unit(Vec2(150.0e9, 0.0), Vec2(0.0, 30822.0), 7.3476e22, "Moon", Vec4(0.2, 0.2, 0.2, 1.0)));
		data.push_back(Unit(Vec2(225.609376e9, 0.0), Vec2(0.0, 26238.0), 1.0659e16, "Phobos", Vec4(0.5, 0.46, 0.3, 1.0)));
		data.push_back(Unit(Vec2(225.623463e9, 0.0), Vec2(0.0, 25451.0), 1.4762e15, "Deimos", Vec4(0.2, 0.8, 0.7, 1.0)));
		data.push_back(Unit(Vec2(778.9217e9, 0.0), Vec2(0.0, 30350.0), 8.931938e22, "Io", Vec4(1.0, 0.2, 0.1, 1.0)));
		data.push_back(Unit(Vec2(779.1709e9, 0.0), Vec2(0.0, 26790.0), 4.799844e22, "Europa", Vec4(1.0, 0.8, 0.7, 1.0)));
		data.push_back(Unit(Vec2(779.5704e9, 0.0), Vec2(0.0, 23930.0), 1.4819e23, "Ganymede", Vec4(0.6, 0.76, 0.95, 1.0)));
		data.push_back(Unit(Vec2(780.3827e9, 0.0), Vec2(0.0, 21254.0), 1.075938e23, "Callisto", Vec4(0.4, 0.1, 0.92, 1.0)));

		//Convert to correct units
		for (auto& unit : data)
		{
			unit.pos *= 1 / physicsUnits::AUtoM;
			unit.vel *= 1 / physicsUnits::AUpYtoMpS;
			unit.mass /= physicsUnits::SMtoKG;
		}
		return data;
	}
}
