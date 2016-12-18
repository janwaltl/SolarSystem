#include <chrono>
#include <iostream>
#include "Simulation.h"
#include "Parsers/Parser.h"
#include "SimMethods/SimMethod.h"
#include "Viewers/Viewer.h"
#include <thread>
#include "Exception.h"

namespace solar
{
	using namespace std::chrono_literals;

	Simulation::Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer) :
		parser(std::move(parser)), simMethod(std::move(simMethod)), viewer(std::move(viewer)),
		state(notRunning)
	{
		LinkUnitAndSim(*this->parser.get(), *this);
		LinkUnitAndSim(*this->viewer.get(), *this);
		LinkUnitAndSim(*this->simMethod.get(), *this);
	}

	void Simulation::Start(stepTime_t dt, std::chrono::seconds maxSimT /*= 0s*/)
	{
		dtime = dt;
		this->maxSimTime = maxSimT;


		//Obtain the data, throws on invalid input(format)
		data = parser->Load();
		simMethod->Prepare(data);
		viewer->Prepare(data);
		Loop();
	}

	void Simulation::StopSimulation()
	{
		state = notRunning;
	}

	void Simulation::PauseSimulation()
	{
		state = paused;
	}

	void Simulation::ResumeSimulation()
	{
		assert(state == paused); //Only paused sim can be resumed
		state = running;
	}

	bool Simulation::IsPaused()
	{
		return state==paused;
	}

	bool Simulation::IsRunnig()
	{
		return state==running;
	}

	double Simulation::GetDtime()
	{
		return ToSecs(dtime);
	}

	double Simulation::GetRunTime()
	{
		return ToSecs(runTime);
	}

	double Simulation::GetSimTime()
	{
		return ToSecs(simTime);
	}

	double Simulation::GetFrameTime()
	{
		return ToSecs(frameTime);
	}

	void Simulation::Loop()
	{
		ResetTimers();

		state = running;
		while (state != notRunning && IsNotRunningForTooLong())
		{
			TickTime();
			while (acc > dtime)
			{
				for (int i = 0; i < 100; i++)
				{
					(*simMethod)(data, ToSecs(30min) / physicsUnits::YtoS);//Step in years
					simTime += 30min;
				}
				acc -= dtime;
			}

			(*viewer)(data);
		}
		state = notRunning;
		parser->Save(data);
	}

	void Simulation::ResetTimers()
	{
		acc = decltype(acc)::zero();
		simTime = decltype(simTime)::zero();
		runTime = decltype(runTime)::zero();
		begining = prevTime = clock_t::now();
	}

	void Simulation::TickTime()
	{
		auto now = clock_t::now();
		frameTime = now - prevTime;
		prevTime = now;
		runTime = now - begining;

		if (state == paused)
			acc = acc.zero();
		else if (frameTime > 1s)
			acc += 1s;
		else
			acc += frameTime;
	}

	bool Simulation::IsNotRunningForTooLong()
	{
		return (maxSimTime == decltype(maxSimTime)::zero() || runTime < maxSimTime);
	}
}
