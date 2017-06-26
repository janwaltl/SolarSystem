#include "Unit.h"
#include "PhysicsUnits.h"

#include <algorithm>

namespace solar
{
	SimData::units_t * SimData::operator->()
	{
		return &units;
	}
	const SimData::units_t * SimData::operator->() const
	{
		return &units;
	}
	SimData::units_t & SimData::operator*()
	{
		return units;
	}
	const SimData::units_t & SimData::operator*() const
	{
		return units;
	}
	SimData::units_t & SimData::Get()
	{
		return units;
	}
	const SimData::units_t & SimData::Get() const
	{
		return units;
	}
	void SimData::SetPhysUnits(PhysUnits::ratio newMass, PhysUnits::ratio newDist, PhysUnits::ratio newTime)
	{
		mass = newMass;
		dist = newDist;
		time = newTime;
	}

	void SimData::ConvertTo(PhysUnits::ratio newMass, PhysUnits::ratio newDist, PhysUnits::ratio newTime)
	{
		ConvertMassTo(newMass);
		ConvertDistTo(newDist);
		ConvertTimeTo(newTime);
	}

	void SimData::ConvertMassTo(PhysUnits::ratio newMass)
	{
		if (abs(newMass - mass) > epsilon<PhysUnits::ratio>)
		{
			PhysUnits::ratio ratio = mass / newMass;
			for (auto& unit : units)
				unit.mass *= ratio;
			mass = newMass;
		}
	}

	void SimData::ConvertDistTo(PhysUnits::ratio newDist)
	{
		if (abs(newDist - dist) > epsilon<PhysUnits::ratio>)
		{
			PhysUnits::ratio ratio = dist / newDist;
			for (auto& unit : units)
			{
				unit.pos *= ratio;
				unit.vel *= ratio;
			}
			dist = newDist;
		}
	}

	void SimData::ConvertTimeTo(PhysUnits::ratio newTime)
	{
		if (abs(newTime - time) > epsilon<PhysUnits::ratio>)
		{
			PhysUnits::ratio ratio = time / newTime;
			for (auto& unit : units)
				unit.vel /= ratio;//speed is in denominator
			time = newTime;
		}
	}

	void SimData::Normalize()
	{
		if (units.size() == 0)
			return;

		double maxPos, maxMass, maxVel;
		maxPos = maxMass = maxVel = std::numeric_limits<double>::min();
		for (const auto& unit : units)
		{
			maxPos = std::max(maxPos, abs(unit.pos.x));
			maxPos = std::max(maxPos, abs(unit.pos.y));
			maxVel = std::max(maxVel, abs(unit.vel.x));
			maxVel = std::max(maxVel, abs(unit.vel.y));
			maxMass = std::max(maxMass, abs(unit.mass));
		}
		ConvertTo(mass*maxMass, dist*maxPos, time* maxPos / maxVel);
	}

	PhysUnits::ratio SimData::RatioOfMassTo(PhysUnits::ratio newMass)
	{
		return mass / newMass;
	}

	PhysUnits::ratio SimData::RatioOfDistTo(PhysUnits::ratio newDist)
	{
		return dist / newDist;
	}

	PhysUnits::ratio SimData::RatioOfTimeTo(PhysUnits::ratio newTime)
	{
		return time / newTime;
	}
}
