#include <chrono>
#include <iostream>
#include "Simulation.h"
#include "Parsers/Parser.h"
#include "SimMethods/SimMethod.h"
#include "Viewers/Viewer.h"
#include <thread>

using namespace std::chrono_literals;

Simulation::Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer) :
	parser(std::move(parser)), simMethod(std::move(simMethod)), viewer(std::move(viewer)),
	running(false)
{
	this->parser->Link(this);
	this->viewer->Link(this);
	this->simMethod->Link(this);
}

void Simulation::Start(stepTime_t dt, std::chrono::seconds maxSimTime /*= 0s*/)
{
	dtime = dt;
	this->maxSimTime = maxSimTime;

	//Obtain the data
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

		auto tmp = acc;

		while (acc > dtime)
		{
			//(*simMethod)(data, dtime);
			simTime += dtime;
			acc -= dtime;
		}
		std::cout << "\t Simulated Time" << (tmp - acc).count() / double(decltype(tmp)::period::den) << std::endl;

		(*viewer)(data);
	}
	running = false;

	std::cout << "RunTime" << runTime.count() / double(decltype(runTime)::period::den)
			  << "s  Simulated amount " << simTime.count() / double(decltype(simTime)::period::den) << "s\n";
	
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

	std::cout << "FrameTime: " << frameTime.count() / double(decltype(frameTime)::period::den) << "secs\n";
	return frameTime;
}

bool Simulation::IsNotRunningForTooLong()
{
	return (maxSimTime == decltype(maxSimTime)::zero() || runTime < maxSimTime);
}

