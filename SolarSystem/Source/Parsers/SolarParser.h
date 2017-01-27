#ifndef PARSERS_SOLAR_PARSER_HEADER
#define PARSERS_SOLAR_PARSER_HEADER

#include "Parser.h"

namespace solar
{
	//Loads hard-coded data of solar system
	class SolarParser :public Parser
	{
	public:
		simData_t Load() override final;
	};
}


#endif