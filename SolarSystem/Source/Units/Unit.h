#ifndef ELEMENTS_ELEMENT_HEADER
#define ELEMENTS_ELEMENT_HEADER

#include <string>
#include <vector>
#include "Source/Math/Math.h"
#include "PhysicsUnits.h"

namespace solar
{
	//Basic unit of simulated data - planet, star, moon...
	struct Unit
	{
	public:
		explicit Unit(const Vec3d& pos = Vec3d(), const Vec3d&vel = Vec3d(), double mass = 0.0, const std::string& name = "",
					  const Vec4d& color = Vec4d(1.0, 1.0, 1.0, 1.0))
			:pos(pos), vel(vel), mass(mass), name(name), color(color)
		{}

		Vec3d pos, vel;
		double mass;
		Vec4d color;
		std::string name;
	};

	class SimData
	{
	public:
		using units_t = std::vector<Unit>;
		//Quick access to vector of simulated units
		units_t* operator->();
		const units_t* operator->() const;
		//Quick access to vector of simulated units
		units_t& operator*();
		const units_t& operator*() const;
		//Access to vector of simulated units
		units_t& Get();
		const units_t& Get() const;
		//Set units in which are simData
		void SetPhysUnits(PhysUnits::ratio newMass, PhysUnits::ratio newDist, PhysUnits::ratio newTime);
		//Converts simData to desired physics units
		void ConvertTo(PhysUnits::ratio newMass, PhysUnits::ratio newDist, PhysUnits::ratio newTime);
		void ConvertMassTo(PhysUnits::ratio newMass);
		void ConvertDistTo(PhysUnits::ratio newDist);
		void ConvertTimeTo(PhysUnits::ratio newTime);
		//Converts to such units that all values lie in [-1.0,1.0]
		void Normalize();
		//Gets ratio to multiply simData to get them to desired physics units
		PhysUnits::ratio RatioOfMassTo(PhysUnits::ratio newMass);
		PhysUnits::ratio RatioOfDistTo(PhysUnits::ratio newDist);
		PhysUnits::ratio RatioOfTimeTo(PhysUnits::ratio newTime);
	private:
		units_t units;
		PhysUnits::ratio mass, dist, time;
	};
}


#endif