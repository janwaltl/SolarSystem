#ifndef PARSERS_REPLAYER_PARSER_HEADER
#define PARSERS_REPLAYER_PARSER_HEADER

#include "Parser.h"
namespace solar
{
	class ReplayerParser : public Parser
	{
	public:
		//replayFile - filename including path and extension
		//				must be .replay file containing proper replay
		ReplayerParser(const std::string& replayFileName);
		//Loads data from replayFile
		simData_t Load() override final;
		//No save

	private:
		std::string replayFile;
	};
}


#endif