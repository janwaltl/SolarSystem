#ifndef PARSERS_PARSER_HEADER
#define PARSERS_PARSER_HEADER

#include "Source/Units/SystemUnit.h"
#include "Source/Units/Unit.h"

namespace solar
{
	//Loads simulated data for the simulation and optionally also saves them after the simulation finished.
	class Parser : public SystemUnit
	{
	public:
		//Loads input from whethever
		virtual SimData Load() = 0;
		//Save them after end of simulation
		virtual void Save(const SimData&) {};
		virtual ~Parser() = default;
	};
}


#endif