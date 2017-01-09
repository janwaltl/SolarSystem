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
	void SystemUnit::SetDTime(double newDT)
	{
		sim->SetDTime(newDT);
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
	size_t SystemUnit::GetDTMultiplier()
	{
		return sim->GetDTMultiplier();
	}
	size_t SystemUnit::GetRawMultiplier()
	{
		return sim->GetRawMultiplier();;
	}
	void SystemUnit::SetRawMultiplier(size_t newRawMult)
	{
		sim->SetRawMultiplier(newRawMult);
	}
	void SystemUnit::SetDTMultiplier(size_t newDTMult)
	{
		sim->SetDTMultiplier(newDTMult);
	}
	void SystemUnit::PauseSimulation()
	{
		sim->PauseSimulation();
	}
	void SystemUnit::ResumeSimulation()
	{
		sim->ResumeSimulation();
	}
	void SystemUnit::StepSimulation()
	{
		sim->StepSimulation();
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