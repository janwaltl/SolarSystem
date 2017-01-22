#include <chrono>
#include <iostream>

#include "Simulation.h"
#include "Parsers/FormattedFileParser.h"

#include "SimMethods/RK4.h"
#include "SimMethods/SemiImplicitEuler.h"

#include "Viewers/IMGuiViewer.h"
#include "Viewers/EmptyViewer.h"

#include "Exception.h"

#include "RecordedSimulation.h"
#include "ReplayedSimulation.h"

int main()
{
	using namespace solar;
	using namespace std::chrono_literals;
	std::ios_base::sync_with_stdio(false);
	try
	{
		try
		{
			auto parser = std::make_unique<FormattedFileParser>("vstup.txt");
			//auto viewer = std::make_unique<IMGuiViewer>(1200, 700, "Title");
			auto method = std::make_unique<RK4>();

			//Simulation sim(std::move(parser), std::move(method), std::move(viewer));
			//Start with timeStep of 10ms, speed it up 100 times and pass 10ms*180 000=30min dTime
			// Thus effectively running 3000min simTime for each 10ms realTime = 208days per second
			//sim.Start(10ms, 50, 180'000, 300s);

			///Recorded simulation
			/*RecordedSimulation recSim(std::move(parser), std::move(method),
									  std::make_unique<EmptyViewer>(), "out.replay");
			recSim.Start(10ms, 10, 180'000, 300s);*/

			///Replayed sim
			ReplayedSimulation repSim("out.replay");
			repSim.Start(10ms, 1, 1, 300s);

			//std::cin.get();
			//std::cin.get();
		}
		catch (const std::exception& e)
		{
			std::cout << "Simulation failed, reason:\n" << e.what();
			system("PAUSE");
		}
	}
	catch (...)
	{
		std::cout << "UNCAUGHT EXCEPTION";
	}
	return true;
}