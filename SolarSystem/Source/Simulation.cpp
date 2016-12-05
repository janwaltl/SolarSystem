#include <chrono>
#include "Simulation.h"
#include "Parsers/Parser.h"
#include "SimMethods/SimMethod.h"
#include "Viewers/Viewer.h"


using namespace std::chrono_literals;

Simulation::Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer) :
	parser(std::move(parser)), simMethod(std::move(simMethod)), viewer(std::move(viewer)),
	running(false)
{
	parser->Link(this);
	viewer->Link(this);
	simMethod->Link(this);
}

void Simulation::Start(double dt, runTime_t maxSimTime /*= 0s*/)
{
	dtime = dt;
	this->maxSimTime = maxSimTime;

	elements = parser->Load();

	Loop();
}

void Simulation::StopSimulation()
{
	running = false;
}

void Simulation::Loop()
{
	prevTime = std::chrono::steady_clock::now();
	runTime = runTime_t(0);
	running = true;
	while (running && (maxSimTime == runTime_t(0) || runTime < maxSimTime))
	{
		auto now = std::chrono::steady_clock::now();
		auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - prevTime);
		runTime += std::chrono::duration_cast<runTime_t>(frameTime);
		acc += frameTime.count() / 1000.0;
		acc = acc > 0.5 ? 0.5 : acc;
		while (acc > dtime)
		{
			(*simMethod)(elements, dtime);
			acc -= dtime;
		}

		(*viewer)(elements);
	}
}

