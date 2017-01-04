#ifndef PARSERS_FORMATTED_FILE_PARSER
#define PARSERS_FORMATTED_FILE_PARSER

#include "Parser.h"
#include <string>

namespace solar
{
	class FormattedFileParser :public Parser
	{
	public:
		//Including extensions, leave outFileName empty if you don't want to save finished simulation
		FormattedFileParser(const std::string& inputFileName,const std::string& outputFileName="");
		//Data's values are NOT in SI units,
		//pos is in AU units, velocity in AU/Earth years and mass in multiples of Sun's mass
		//Default color is white and pos, mass, vel=0
		simData_t Load() override final;
		void Save(const simData_t& data) override final;
	private:
		static Unit ParseUnit(const std::string& str);
		//Str must in format of' token=<val>', where val is returned string
		//Throws Exception if invalid
		static std::string ParseToken(const std::string&str, const std::string& token);

		static void ParsePosition(Unit& unit, const std::string& val);
		static void ParseVelocity(Unit& unit, const std::string& val);
		static void ParseMass(Unit& unit, const std::string& val);
		static void ParseColor(Unit& unit, const std::string& val);
		static void ParseName(Unit& unit, const std::string& val);
		static std::string SerializeUnit(const Unit& unit);

		const std::string inFileName, outFileName;
		std::string input;
	};
}

#endif