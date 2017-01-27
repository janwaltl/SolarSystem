#ifndef SIMMETHODS_REPLAYER_METHOD_HEADER
#define SIMMETHODS_REPLAYER_METHOD_HEADER

#include "SimMethod.h"
#include <fstream>

namespace solar
{
	//Part of replaying a simulation mode.
	//Simulates data, by loading correct record from file based on simulated time.
	class ReplayerSimMethod :public SimMethod
	{
	public:
		//From which file load simulated data, including path and extension
		ReplayerSimMethod(const std::string& inFileName);
		void Prepare() override final;
		// Pretends to simulate data, in reality loads correct state of simulation from file based on simTime
		void operator()(double)override final;
	private:
		std::string inFile;
		std::ifstream in;
		//Byte offset to first record at time zero
		uint32_t offset;
		uint32_t numRecords;
		uint32_t multiplier;
	};
}


#endif