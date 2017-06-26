#ifndef PARSERS_REPLAYER_PARSER_HEADER
#define PARSERS_REPLAYER_PARSER_HEADER

#include "Parser.h"
namespace solar
{
	//Part of replaying a simulation mode.
	//Responsible for loading correct replay file and initializing data based on it.
	class ReplayerParser : public Parser
	{
	public:
		//replayFile - filename including path and extension(does not have to be '.replay')
		//			 - file must contain proper replay
		//				- IS checked by constructor and Exception class is thrown on error
		ReplayerParser(const std::string& replayFileName);
		//Loads data from replayFile
		SimData Load() override final;
		//No save
	private:
		std::string inFile;
	};
}


#endif