#include <chrono>
#include <iostream>

#include "Simulation.h"
#include "Viewers/IMGuiViewer.h"
#include "Parsers/FormattedFileParser.h"
#include "SimMethods/RK4.h"
#include "SimMethods/SemiImplicitEuler.h"
#include "Exception.h"
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
			auto viewer = std::make_unique<IMGuiViewer>(1200, 700, "Title", 0.005f, 12);
			auto method = std::make_unique<RK4>();

			Simulation sim(std::move(parser), std::move(method), std::move(viewer));
			//Start with timeStep of 10ms, speed it up 100 times and pass 10ms*180 000=30min dTime
			// Thus effectively running 3000min simTime for each 10ms realTime = 208days per second
			sim.Start(10ms, 100, 180'000, 300s);
			//std::cin.get();
			//std::cin.get();
		}
		catch (const Exception& e)
		{
			std::cout << "Simulation failed, reason:" << e.what();
			system("PAUSE");
		}
	}
	catch (...)
	{
		std::cout << "UNCAUGHT EXCEPTION";
	}
	return true;
}