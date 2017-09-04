#ifndef PARSERS_FORMATTED_FILE_PARSER
#define PARSERS_FORMATTED_FILE_PARSER

#include "Parser.h"
#include <string>

namespace solar
{
	//Loads simulated data from formatted text files
	//Outputed simData's values are NOT in SI units,
	// - pos is in AU units, velocity in AU/Earth years and mass in multiples of Sun's mass
	// - Default color is white and pos=mass=vel=0, radius=1.0
	//For detailed description of format, see documentation of examples included with binaries.
	//Throws 'Exception' on any failure
	class FormattedFileParser :public Parser
	{
	public:
		//Including path and extension, leave outFileName empty if you don't want to save finished simulation
		FormattedFileParser(const std::string& inputFileName, const std::string& outputFileName = "");
		SimData Load() override final;
		void Save(const SimData& data) override final;
	private:
		static Unit ParseUnit(const std::string& str);
		//Str must be in format of' token=<val>', where val is returned string
		//Throws Exception if invalid
		static std::string ParseToken(const std::string&str, const std::string& token);

		static void ParsePosition(Unit& unit, const std::string& val);
		static void ParseVelocity(Unit& unit, const std::string& val);
		static void ParseMass(Unit& unit, const std::string& val);
		static void ParseRadius(Unit& unit, const std::string& val);
		static void ParseColor(Unit& unit, const std::string& val);
		static void ParseName(Unit& unit, const std::string& val);
		const std::string inFileName, outFileName;
		std::string input;
	};
}

#endif