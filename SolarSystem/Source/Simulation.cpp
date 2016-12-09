#include <chrono>
#include <iostream>
#include "Simulation.h"
#include "Parsers/Parser.h"
#include "SimMethods/SimMethod.h"
#include "Viewers/Viewer.h"
#include <thread>
#include "Exception.h"

using namespace std::chrono_literals;

Simulation::Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer) :
	parser(std::move(parser)), simMethod(std::move(simMethod)), viewer(std::move(viewer)),
	running(false)
{
	this->parser->Link(this);
	this->viewer->Link(this);
	this->simMethod->Link(this);
}

void Simulation::Start(stepTime_t dt, std::chrono::seconds maxSimT /*= 0s*/)
{
	dtime = dt;
	this->maxSimTime = maxSimT;


	//Obtain the data, throws on invalid input(format)
	data = parser->Load();

	Loop();
}

void Simulation::StopSimulation()
{
	running = false;
}

void Simulation::Loop()
{
	ResetTimers();

	running = true;
	while (running && IsNotRunningForTooLong())
	{
		TickTime();

		auto tmp = acc;///LOGGING
		while (acc > dtime)
		{
			(*simMethod)(data, ToSecs(dtime));
			simTime += dtime;
			acc -= dtime;
		}
		///LOGGING
		std::cout << "\t Simulated Time" << ToSecs(tmp - acc) << std::endl;

		(*viewer)(data);
	}
	running = false;
	///LOGGING
	std::cout << "RunTime" << ToSecs(runTime) << "s  Simulated amount " << ToSecs(simTime) << "s\n";
	parser->Save(data);
}

void Simulation::ResetTimers()
{
	acc = decltype(acc)::zero();
	simTime = decltype(simTime)::zero();
	runTime = decltype(runTime)::zero();
	begining = prevTime = clock_t::now();
}

Simulation::stepTime_t Simulation::TickTime()
{
	auto now = clock_t::now();
	auto frameTime = now - prevTime;
	prevTime = now;
	runTime = now - begining;
	acc += frameTime;
	///LOGGING
	std::cout << "FrameTime: " << frameTime.count() / double(decltype(frameTime)::period::den) << "secs\n";
	return frameTime;
}

bool Simulation::IsNotRunningForTooLong()
{
	return (maxSimTime == decltype(maxSimTime)::zero() || runTime < maxSimTime);
}

