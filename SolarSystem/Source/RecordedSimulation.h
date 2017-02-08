#ifndef RECORDER_SIMULATION_HEADER
#define RECORDER_SIMULATION_HEADER

#include "Simulation.h"
#include "Viewers/ViewAndRecord.h"

namespace solar
{
	//Wrapper around Simulation, that automatically also records it
	//DO NOT CHANGE SimTime while recording via simMethod or viewer.
	class RecordedSimulation
	{
	public:
		//Outfile - name of a file where simulation should be saved, including path and extension
		RecordedSimulation(parser_p parser, simMethod_p simMethod, viewer_p viewer, const std::string& outFile) :
			sim(std::move(parser), std::move(simMethod),
				std::make_unique<ViewAndRecord>(outFile, std::move(viewer)))
		{
		}
		//Starts simulation - loops until stopped, or maxRunTime is reached
		//Throws Exception(std::logic_error) on invalid input or if any other SystemUnit throws.
		//dTime - how often simMethod is called 
		//rawMult - how many times is simMethod called for each dTime
		//DTMult - how many dTimes are passed to simMethod.
		//example: dTime=1ms; rawMult=20;DTMult=5
		//			 Means that each 1ms simMethod will be called 20times with 5ms being passed as dTime to it
		void Start(stepTime_t dt, size_t rawMult = 1, size_t DTMult = 1,
				   std::chrono::seconds maxRunTime = std::chrono::seconds::zero())
		{
			sim.Start(dt, rawMult, DTMult, maxRunTime);
		}
		//Starts simulation - loops until stopped, or maxRunTime is reached.
		//Throws Exception(std::logic_error) on invalid input or if any other SystemUnit throws.
		//dTime - how often simMethod is called
		//rawMult - how many times is simMethod called for each viewer call.
		void StartNotTimed(stepTime_t dt, size_t rawMult = 1,
						   std::chrono::seconds maxRunTime = std::chrono::seconds::zero())
		{
			sim.StartNotTimed(dt, rawMult, maxRunTime);
		}
	private:
		Simulation sim;
	};
}


#endif