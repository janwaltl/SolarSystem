#ifndef PARSERS_SOLAR_PARSER_HEADER
#define PARSERS_SOLAR_PARSER_HEADER

#include "Parser.h"
#include <string>

namespace solar
{
	//Loads hard-coded data of solar system
	class SolarParser :public Parser
	{
	public:
		explicit SolarParser(const std::string& saveFile = "");
		SimData Load() override final;
		void Save(const SimData& data) override final;
	private:
		const std::string saveFile;
	};
}


#endif