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
		simData_t Load() override final;
		void Save(const simData_t& data) override final;
	private:
		Unit ParseUnit(const std::string& str);
		//Str must in format of' token=<val>', where val is returned string
		//Throws Exception if invalid
		std::string ParseToken(const std::string&str, const std::string& token);

		void ParsePosition(Unit& unit, const std::string& val);
		void ParseVelocity(Unit& unit, const std::string& val);
		void ParseMass(Unit& unit, const std::string& val);

		std::string SerializeUnit(const Unit& unit);
		const std::string inFileName, outFileName;
		std::string input;
	};
}

#endif