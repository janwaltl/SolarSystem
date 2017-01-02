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

	void Simulation::Start(stepTime_t dt, size_t rawMult /*= 1*/, size_t DTMult /*= 1*/, std::chrono::seconds maxSimT /*= 0s*/)
	{
		dtime = dt;
		maxSimTime = maxSimT;
		rawMultiplier = rawMult;
		DTMultiplier = DTMult;

		//Obtain the data, throws on invalid input(format)
		data = parser->Load();
		simMethod->_Prepare(&data);
		viewer->_Prepare(&data);
		Loop();
		parser->Save(data);

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

	void Simulation::StepSimulation()
	{
		state = stepping;
	}

	bool Simulation::IsPaused()
	{
		return state == paused;
	}

	bool Simulation::IsRunnig()
	{
		return state == running;
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

	size_t Simulation::GetRawMultiplier()
	{
		return rawMultiplier;
	}

	size_t Simulation::GetDTMultiplier()
	{
		return DTMultiplier;
	}

	void Simulation::SetRawMultiplier(size_t newRawMult)
	{
		rawMultiplier = newRawMult;
	}

	void Simulation::SetDTMultiplier(size_t newDTMult)
	{
		DTMultiplier = newDTMult;
	}

	void Simulation::Loop()
	{
		ResetTimers();

		state = running;
		while (state != notRunning && IsNotRunningForTooLong())
		{
			TickTime();
			while (acc > dtime && state!=paused)
			{
				for (size_t i = 0; i < rawMultiplier; i++)
				{
					(*simMethod)(ToSecs(dtime * DTMultiplier) / physicsUnits::YtoS);//Step in years
					simTime += dtime*DTMultiplier;
				}
				acc -= dtime;

				if (state == stepping)// If we were stepping, we just made a step, so pause a simulation
					state = paused;
			}

			(*viewer)();
		}
		state = notRunning;
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

		if (frameTime > 500ms)
			acc += 500ms;
		else
			acc += frameTime;
	}

	bool Simulation::IsNotRunningForTooLong()
	{
		return (maxSimTime == decltype(maxSimTime)::zero() || runTime < maxSimTime);
	}
}
