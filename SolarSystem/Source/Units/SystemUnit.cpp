#include "SystemUnit.h"

#include <cassert>
#include "Source/Simulation.h"

namespace solar
{
	void SystemUnit::StopSimulation()
	{
		assert(sim);
		sim->StopSimulation();
	}
	double SystemUnit::GetDtime() const
	{
		assert(sim);
		return sim->GetDtime();
	}
	void SystemUnit::SetDTime(double newDT)
	{
		sim->SetDTime(newDT);
	}
	double SystemUnit::GetRunTime() const
	{
		return sim->GetRunTime();
	}
	const simulatedTime& SystemUnit::GetSimTime() const
	{
		return sim->GetSimTime();
	}
	double SystemUnit::GetSimTimeSecs() const
	{
		const auto& time = sim->GetSimTime();
		return ToSecs(time);
	}
	void SystemUnit::SetSimTime(simulatedTime newSimTime)
	{
		sim->SetSimTime(newSimTime);
	}
	double SystemUnit::GetFrameTime() const
	{
		return sim->GetFrameTime();;
	}
	size_t SystemUnit::GetDTMultiplier() const
	{
		return sim->GetDTMultiplier();
	}
	size_t SystemUnit::GetRawMultiplier() const
	{
		return sim->GetRawMultiplier();
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