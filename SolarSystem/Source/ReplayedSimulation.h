#ifndef REPLAYED_SIMULATION_HEADER
#define REPLAYED_SIMULATION_HEADER
#endif

#include "Simulation.h"
#include "Parsers/ReplayerParser.h"
#include "SimMethods/ReplayerMethod.h"
#include "Viewers/ReplayerViewer.h"
namespace solar
{
	//Replays recorded simulations
	class ReplayedSimulation
	{
	public:
		//replaFile - name of file containing replay, including path and extension
		ReplayedSimulation(const std::string& replayFile, size_t winWidth, size_t winHeight, const std::string& title) :
			sim(std::make_unique<ReplayerParser>(replayFile),
				std::make_unique<ReplayerSimMethod>(replayFile),
				std::make_unique<ReplayerViewer>(replayFile, winWidth, winHeight, title))
		{
		}
		void Start()
		{
			using namespace std::chrono_literals;
			sim.Start(10ms);//Does not matter, will be changed by parser to correct one
		}
	private:
		Simulation sim;
	};
}