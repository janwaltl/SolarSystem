#ifndef SERIALIZE_UNIT_HEADER
#define SERIALIZE_UNIT_HEADER

#include <fstream>
#include "Source/Units/Unit.h"
#include "Source/Units/PhysicsUnits.h"

namespace solar
{
	//Serializes passed Unit, values are written without regard to physics units
	inline std::ostream& operator<< (std::ostream& out, const Unit& unit)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt
		out << "{ name<" << unit.name << ">\n";
		out << "  color<" << unit.color.x << " " << unit.color.y << " " << unit.color.z << " " << unit.color.w << ">\n";
		out << "  position<" << unit.pos.x << " " << unit.pos.y << " " << unit.pos.z << ">\n";
		out << "  velocity<" << unit.vel.x << " " << unit.vel.y << " " << unit.vel.z << ">\n";
		out << "  mass<" << unit.mass << ">}\n";

		return out;
	}
	//Serializes passed Unit, values are written in desired outputUnits
	inline void PrintUnit(std::ostream& out, const Unit& unit, const PhysUnits& currUnits, PhysUnits& outputUnits)
	{
		auto massRatio = currUnits.mass / outputUnits.mass;
		auto velRatio = currUnits.dist / currUnits.time / (outputUnits.dist / outputUnits.time);
		auto distRatio = currUnits.dist / outputUnits.dist;
		out << "{ name<" << unit.name << ">\n";
		out << "  color<" << unit.color.x << " " << unit.color.y << " " << unit.color.z << " " << unit.color.w << ">\n";
		out << "  position<" << unit.pos.x*distRatio << " " << unit.pos.y*distRatio << " " << unit.pos.z*distRatio << ">\n";
		out << "  velocity<" << unit.vel.x *velRatio << " " << unit.vel.y*velRatio << " " << unit.vel.z*velRatio << ">\n";
		out << "  mass<" << unit.mass*massRatio << ">}\n";
	}
}
#endif