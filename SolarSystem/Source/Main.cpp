#include <chrono>
#include <iostream>

#include "Simulation.h"
#include "Parsers/FormattedFileParser.h"

#include "SimMethods/RK4.h"
#include "SimMethods/SemiImplicitEuler.h"

#include "Viewers/IMGuiViewer.h"
#include "Viewers/EmptyViewer.h"

#include "Source/Common/Exception.h"

#include "RecordedSimulation.h"
#include "ReplayedSimulation.h"
#include "ConsoleControl.h"

#define CONSOLE

int main(int argc, char * argv[])
{
	using namespace solar;
	using namespace std::chrono_literals;
	try
	{
		try
		{
#ifdef SIMULATE
			Simulation sim(std::make_unique<FormattedFileParser>("vstup.txt"),
						   std::make_unique<IMGuiViewer>(1200, 700, "Title"),
						   std::make_unique<RK4>());
			sim.Start(10ms, 50, 180'000, 300s);
#endif
#ifdef RECORD
			RecordedSimulation recSim(std::make_unique<FormattedFileParser>("vstup.txt"),
									  std::make_unique<RK4>(),
									  std::make_unique<EmptyViewer>(),
									  "out.replay");

			recSim.Start(10ms, 100, 180'000, 300s);
#endif
#ifdef REPLAY
			ReplayedSimulation repSim("out.replay");
			repSim.Start(10ms, 1, 1, 300s);
#endif
#ifdef CONSOLE
			console::Control(argc, argv);
#endif
		}
		catch (const std::exception& e)
		{
			std::cout << "Following exception has been thrown:\n" << e.what()<<"\n";
			system("PAUSE");
		}
	}
	catch (...)
	{
		std::cout << "UNCAUGHT EXCEPTION!";
	}
	return 0;
}


