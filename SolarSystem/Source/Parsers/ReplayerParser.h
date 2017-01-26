#ifndef PARSERS_REPLAYER_PARSER_HEADER
#define PARSERS_REPLAYER_PARSER_HEADER

#include "Parser.h"
namespace solar
{
	//Responsible for loading correct replay file and initializing data based on it.
	class ReplayerParser : public Parser
	{
	public:
		//replayFile= filename including path and extension
		//			- file must contain proper replay, .replay extension is not needed
		//				- IS checked by constructor and Exception class is thrown on error
		ReplayerParser(const std::string& replayFileName);
		//Loads data from replayFile
		simData_t Load() override final;
		//No save

	private:
		std::string inFile;
	};
}


#endif