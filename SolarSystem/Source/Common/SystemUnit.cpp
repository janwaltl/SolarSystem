#include "SystemUnit.h"
#include "../Simulation.h"

namespace solar
{

	void SystemUnit::StopSimulation()
	{
		assert(sim);
		sim->StopSimulation();
	}
	double SystemUnit::GetDtime()
	{
		assert(sim);
		return sim->GetDtime();
	}
	double SystemUnit::GetRunTime()
	{
		return sim->GetRunTime();
	}
	double SystemUnit::GetSimTime()
	{
		return sim->GetSimTime();
	}
	double SystemUnit::GetFrameTime()
	{
		return sim->GetFrameTime();;
	}
	void SystemUnit::PauseSimulation()
	{
		sim->PauseSimulation();
	}
	void SystemUnit::ResumeSimulation()
	{
		sim->ResumeSimulation();
	}
	bool SystemUnit::IsRunning()
	{
		return sim->IsRunnig();
	}
	bool SystemUnit::IsPaused()
	{
		return sim->IsPaused();
	}
}