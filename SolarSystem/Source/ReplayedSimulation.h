#ifndef REPLAYED_SIMULATION_HEADER
#define REPLAYED_SIMULATION_HEADER
#endif

#include "Simulation.h"
#include "Parsers/ReplayerParser.h"
#include "SimMethods/ReplayerMethod.h"

namespace solar
{
	//Replays recorded simulations
	class ReplayedSimulation :public Simulation
	{
	public:
		//replaFile - name of file containing replay, including path and extension
		ReplayedSimulation(const std::string& replayFile, viewer_p viewer) :
			Simulation(std::make_unique<ReplayerParser>(replayFile),
					   std::make_unique<ReplayerSimMethod>(replayFile),
					   std::move(viewer))
		{
		}
	};
}