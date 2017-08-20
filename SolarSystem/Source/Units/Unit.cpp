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
	Unit & SimData::operator[](size_t index)
	{
		return units[index];
	}
	const Unit & SimData::operator[](size_t index) const
	{
		return units[index];
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
		physUnits.mass = newMass;
		physUnits.dist = newDist;
		physUnits.time = newTime;
	}

	void SimData::SetPhysUnits(const PhysUnits & newPhysUnits)
	{
		physUnits = newPhysUnits;
	}

	const PhysUnits & SimData::GetPhysUnits() const
	{
		return physUnits;
	}

	void SimData::ConvertTo(PhysUnits::ratio newMass, PhysUnits::ratio newDist, PhysUnits::ratio newTime)
	{
		ConvertMassTo(newMass);
		ConvertDistTo(newDist);
		ConvertTimeTo(newTime);
	}

	void SimData::ConvertMassTo(PhysUnits::ratio newMass)
	{
		if (abs(newMass - physUnits.mass) > epsilon<PhysUnits::ratio>)
		{
			PhysUnits::ratio ratio = physUnits.mass / newMass;
			for (auto& unit : units)
				unit.mass *= ratio;
			physUnits.mass = newMass;
		}
	}

	void SimData::ConvertDistTo(PhysUnits::ratio newDist)
	{
		if (abs(newDist - physUnits.dist) > epsilon<PhysUnits::ratio>)
		{
			PhysUnits::ratio ratio = physUnits.dist / newDist;
			for (auto& unit : units)
			{
				unit.pos *= ratio;
				unit.vel *= ratio;
				unit.radius *= ratio;
			}
			physUnits.dist = newDist;
		}
	}

	void SimData::ConvertTimeTo(PhysUnits::ratio newTime)
	{
		if (abs(newTime - physUnits.time) > epsilon<PhysUnits::ratio>)
		{
			PhysUnits::ratio ratio = physUnits.time / newTime;
			for (auto& unit : units)
				unit.vel /= ratio;//speed is in denominator
			physUnits.time = newTime;
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
		ConvertTo(physUnits.mass*maxMass, physUnits.dist*maxPos, physUnits.time* maxPos / maxVel);
	}

	PhysUnits::ratio SimData::RatioOfMassTo(PhysUnits::ratio newMass) const
	{
		return physUnits.mass / newMass;
	}

	PhysUnits::ratio SimData::RatioOfDistTo(PhysUnits::ratio newDist) const
	{
		return physUnits.dist / newDist;
	}

	PhysUnits::ratio SimData::RatioOfTimeTo(PhysUnits::ratio newTime) const
	{
		return physUnits.time / newTime;
	}
}
