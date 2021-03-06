#include <chrono>
#include <iostream>
#include "Simulation.h"
#include "Parsers/Parser.h"
#include "SimMethods/SimMethod.h"
#include "Viewers/Viewer.h"
#include <thread>
#include "Source/Common/Exception.h"

namespace solar
{
	using namespace std::chrono_literals;

	Simulation::Simulation(parser_p parser, simMethod_p simMethod, viewer_p viewer) :
		parser(std::move(parser)), simMethod(std::move(simMethod)), viewer(std::move(viewer)),
		state(notRunning), dtime(0), rawMultiplier(1), DTMultiplier(1), justUnpaused(false)
	{
		//Links together all SystemUnits.
		//gives viewers and simMethods access to simulated data
		LinkUnitAndSim(*this->parser.get(), *this);
		LinkUnitAndSim(*this->viewer.get(), *this, &data);
		LinkUnitAndSim(*this->simMethod.get(), *this, &data);
	}

	void Simulation::Start(stepTime_t dt, size_t rawMult /*= 1*/, size_t DTMult /*= 1*/, std::chrono::seconds maxRunT /*= 0s*/)
	{
		dtime = dt;
		maxRunTime = maxRunT;
		SetRawMultiplier(rawMult);
		SetDTMultiplier(DTMult);

		//Obtain the data, throws on invalid input(format)
		data = parser->Load();
		simMethod->Prepare();
		viewer->Prepare();
		Loop();
		parser->Save(data);
	}

	void Simulation::StartNotTimed(stepTime_t dt, size_t rawMult, std::chrono::seconds maxRunT)
	{
		dtime = dt;
		maxRunTime = maxRunT;
		SetRawMultiplier(rawMult);
		ResetTimers();

		//Load and prepare data
		data = parser->Load();
		simMethod->Prepare();
		viewer->Prepare();
		LoopNotTimed();
		parser->Save(data);
	}

	void Simulation::LoopNotTimed()
	{
		state = running;
		while (state != notRunning && IsNotRunningForTooLong())
		{
			//Update time
			auto now = clock_t::now();
			runTime = now - begining;

			if (state != paused)
			{
				for (size_t i = 0; i < rawMultiplier; i++)
				{
					(*simMethod)(ToSecs(dtime));
				}
				UpdateSimTime();
			}

			(*viewer)();
		}
		state = notRunning;
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
		justUnpaused = true;
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

	double Simulation::GetDtime() const
	{
		return ToSecs(dtime);
	}

	void Simulation::SetDTime(double newDT)
	{
		dtime = std::chrono::nanoseconds(static_cast<long long>(newDT*std::nano::den));
	}

	double Simulation::GetRunTimeSecs() const
	{
		return ToSecs(runTime);
	}

	stepTime_t Simulation::GetRunTime() const
	{
		return runTime;
	}

	const simulatedTime& Simulation::GetSimTime() const
	{
		return simTime;
	}

	void Simulation::SetSimTime(simulatedTime newSimTime)
	{
		/*using secs_t = decltype(simTimeSecs)::rep;
		using precise_t = decltype(simTimePrecise)::rep;

		auto secs = static_cast<secs_t>(newSimTime);
		auto frac = static_cast<precise_t>((newSimTime - secs)*decltype(simTimePrecise)::period::den);
		simTimeSecs = decltype(simTimeSecs)(secs);
		simTimePrecise = decltype(simTimePrecise)(frac);*/
		simTime = newSimTime;
	}

	double Simulation::GetFrameTime() const
	{
		return ToSecs(frameTime);
	}

	size_t Simulation::GetRawMultiplier() const
	{
		return rawMultiplier;
	}

	size_t Simulation::GetDTMultiplier() const
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
			while (acc > dtime && state != paused)
			{
				for (size_t i = 0; i < rawMultiplier; i++)
				{
					(*simMethod)(ToSecs(dtime * DTMultiplier));
				}
				UpdateSimTime();
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
		acc = acc.zero();
		simTime.seconds = simTime.seconds.zero();
		simTime.fraction = simTime.fraction.zero();
		runTime = runTime.zero();
		begining = prevTime = clock_t::now();
	}

	void Simulation::TickTime()
	{
		auto now = clock_t::now();
		frameTime = now - prevTime;
		prevTime = now;
		runTime = now - begining;

		//Do not accumulate time for first frame after pause
		if (!justUnpaused)
		{
			if (frameTime > 200ms)
				acc += 200ms;
			else
				acc += frameTime;
		}
		else
		{
			acc = acc.zero();
			justUnpaused = false;
		}
	}

	void Simulation::UpdateSimTime()
	{
		simTime.fraction += dtime*DTMultiplier*rawMultiplier;
		//Transfers full seconds from simTimePrecise time to simTimeSecs time
		if (simTime.fraction >= 1s)
		{
			auto secs = std::chrono::duration_cast<std::chrono::seconds>(simTime.fraction);
			simTime.fraction -= secs;
			simTime.seconds += secs;
		}
	}

	bool Simulation::IsNotRunningForTooLong()
	{
		//Either maxRunTime=0 -> not bounded
		//Or non zero-> bounded
		return (maxRunTime == decltype(maxRunTime)::zero() || runTime < maxRunTime);
	}
}
