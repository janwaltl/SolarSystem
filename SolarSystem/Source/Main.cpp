#include <chrono>
#include <iostream>

#include "Simulation.h"
#include "Viewers/IMGuiViewer.h"
#include "Parsers/FormattedFileParser.h"
#include "SimMethods/SemiImplicitEuler.h"
#include "Exception.h"
int main()
{
	using namespace solar;
	using namespace std::chrono_literals;

	try
	{
		try
		{
			auto parser = std::make_unique<FormattedFileParser>("vstup.txt");
			auto viewer = std::make_unique<IMGuiViewer>();
			auto method = std::make_unique<SemiImplicitEuler>();

			Simulation sim(std::move(parser), std::move(method), std::move(viewer));
			sim.Start(10ms, 3s);
			std::cin.get();
			std::cin.get();
		}
		catch (const Exception& e)
		{
			std::cout << "Simulation failed, reason:" << e.what();
		}
	}
	catch (...)
	{
		std::cout << "UNCAUGHT EXCEPTION";
	}
	return false;
}