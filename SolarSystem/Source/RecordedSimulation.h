#ifndef RECORDER_SIMULATION_HEADER
#define RECORDER_SIMULATION_HEADER

#include "Simulation.h"
#include "Viewers/ViewAndRecord.h"

namespace solar
{
	//Wrapper around Simulation, that automatically also records it
	class RecordedSimulation :public Simulation
	{
	public:
		//Outfile - name of a file where simulation should be saved, including path and extension
		RecordedSimulation(parser_p parser, simMethod_p simMethod, viewer_p viewer, const std::string& outFile) :
			Simulation(std::move(parser), std::move(simMethod),
					   std::make_unique<ViewAndRecord>(outFile, std::move(viewer)))
		{
		}
	};
}


#endif