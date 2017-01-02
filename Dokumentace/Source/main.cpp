#include <iostream>
#include <chrono>
#include "Simulation.h"

int main()
{
	//Celý program je zabalen do tohoto namespace
	using namespace solar;
	// Zpřístupňuje jednotky u čísel - např. 10s
	using namespace std::chrono_literals;
	try
	{
		auto parser = std::make_unique<FormattedFileParser>("vstup.txt");
		auto viewer = std::make_unique<IMGuiViewer>(1200, 700, "Title");
		auto method = std::make_unique<RK4>();

		Simulation sim(std::move(parser), std::move(method), std::move(viewer));
		sim.Start(10ms, 100, 180'000, 300s);
	}
	catch (const Exception& e)//Simulace používá vyjímky
	{
		std::cout << "Simulation failed, reason:\n" << e.what();
	}
	return 0;
}