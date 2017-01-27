#ifndef PARSERS_PARSER_HEADER
#define PARSERS_PARSER_HEADER

#include "Source/Common/SystemUnit.h"
#include "Source/Units/Unit.h"

namespace solar
{
	//Cares about input and output
	class Parser : public SystemUnit
	{
	public:
		//Loads input from whethever
		virtual simData_t Load() = 0;
		//Save them after end of simulation
		virtual void Save(const simData_t&) {};
		virtual ~Parser() = default;
	};
}


#endif