#ifndef SERIALIZE_UNIT_HEADER
#define SERIALIZE_UNIT_HEADER

#include <fstream>
#include "Source/Units/Unit.h"
#include "Source/Units/PhysicsUnits.h"

namespace solar
{
	//Serializes passed Unit, values are written in SI units
	//Expects unit's data to be in following units:
	// - pos is in AU units, velocity in AU/Earth years and mass in multiples of Sun's mass
	inline std::ostream& operator<< (std::ostream& out, const Unit& unit)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt
		out << "{ name<" << unit.name << ">\n";
		out << "  color<" << unit.color.x << " " << unit.color.y << " " << unit.color.z << " " << unit.color.w << ">\n";
		out << "  position<" << unit.pos.x*physicsUnits::AUtoM << " " << unit.pos.y*physicsUnits::AUtoM << ">\n";
		out << "  velocity<" << unit.vel.x*physicsUnits::AUpYtoMpS << " " << unit.vel.y*physicsUnits::AUpYtoMpS << ">\n";
		out << "  mass<" << unit.mass*physicsUnits::SMtoKG << ">}\n";

		return out;
	}
}
#endif