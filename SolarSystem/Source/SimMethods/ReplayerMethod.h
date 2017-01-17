#ifndef SIMMETHODS_REPLAYER_METHOD_HEADER
#define SIMMETHODS_REPLAYER_METHOD_HEADER

#include "SimMethod.h"
#include <fstream>
namespace solar
{
	class ReplayerSimMethod :public SimMethod
	{
	public:
		//From which file load simulated data, including path and extension
		ReplayerSimMethod(const std::string& inFileName);
		void Prepare() override final;
		//Pretend to simulate data, in reality loads correct state of simulation from file
		// based on simTime
		void operator()(double )override final;
	private:
		std::string inFile;
		std::ifstream in;
		//Byte offset to first record at time zero
		uint32_t offset;
	};
}


#endif