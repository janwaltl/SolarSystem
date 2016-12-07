#include <chrono>

#include "Simulation.h"
#include "Viewers/TextViewer.h"
#include "Parsers/TestingParser.h"
#include "SimMethods/SemiImplicitEuler.h"
int main()
{

	auto parser = std::make_unique<TestingParser>();
	auto viewer = std::make_unique<TextViewer>();
	auto method = std::make_unique<SemiImplicitEuler>();
	
	using namespace std::chrono_literals;
	Simulation sim(std::move(parser), std::move(method), std::move(viewer));
	sim.Start(0.1, 10s);
	std::cin.get();
	std::cin.get();
	return false;
}