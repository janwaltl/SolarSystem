#include "GUIDrawer.h"

#include "IMGuiLibrary/imgui.h"
#include "../IMGuiViewer.h"
#include <algorithm>

namespace solar
{
	GUIDrawer::GUIDrawer(IMGuiViewer * parent) :
		viewer(parent), follow(false)
	{
		assert(viewer);
	}
	void GUIDrawer::Draw(simData_t& data)
	{
		this->simData = &data;

		//Testing transparent window
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(150, 400), ImGuiSetCond_Once);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f,0.0f,0.0f,0.0f});
		ImGui::Begin("Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("%.6f", length(data[3].pos - data[0].pos));
		ImGui::End();
		ImGui::PopStyleColor();

		ControlsWin();
	}
	void GUIDrawer::ControlsWin()
	{

		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(250, 500), ImGuiSetCond_Once);
		if (ImGui::Begin("Simulation's controls", NULL, ImGuiWindowFlags_NoCollapse |
						 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			SimControls();
			ImGui::Separator();
			ZoomControl();
			if (!follow)
			{
				ManualControls();
				OfferFollow();
			}
			else
				Following();
			ImGui::End();
		}
	}
	void GUIDrawer::SimControls()
	{

		if (viewer->IsRunning())
		{
			if (ImGui::StateButtonWithTooltip("|>##Running", "Pause", false))
				viewer->PauseSimulation();
		}
		else
			if (ImGui::StateButtonWithTooltip("||##Paused", "Resume", true))
				viewer->ResumeSimulation();
		ImGui::SameLine();

		ImGui::Button("<<##Slow-down");
		ImGui::TextTooltipOnHover("Slow down");	ImGui::SameLine();
		ImGui::Button(">>##Speed-up");
		ImGui::TextTooltipOnHover("Speed up");	ImGui::SameLine();
		if (viewer->IsPaused())
		{
			ImGui::Button("->##Step");
			ImGui::TextTooltipOnHover("Makes one step forwards");	ImGui::SameLine();
		}
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Metrics:");
		SimMetrics();
	}
	void GUIDrawer::SimMetrics()
	{
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 120.0f);

		ImGui::Text("RawMultiplier"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("For each Dtime passed time, simMethod will be called this many times.\n"
								  "Controls speed of the simulation at expense of CPU power.");
		ImGui::Text("%10i ", viewer->GetRawMultiplier()); ImGui::NextColumn();
		
		ImGui::Text("DTMultiplier"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("dTime*this will be passed to simMethod as dTime.\n"
								  "Controls speed of the simulation at expense of precision.");
		ImGui::Text("%10i ", viewer->GetDTMultiplier()); ImGui::NextColumn();


		ImGui::Text("Speed ratio"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("This amount of time is simulated each passed second.");
		size_t speedRatio = viewer->GetRawMultiplier()*viewer->GetDTMultiplier();
		auto spDays = (speedRatio % 31'536'000) / 86'400;
		auto spHours = (speedRatio % 86'400) / 3600;
		auto spMins = (speedRatio % 3600) / 60;
		auto spSecs = speedRatio % 60;
		ImGui::Text("%id %ih %im %is", spDays, spHours, spMins, spSecs); ImGui::NextColumn();
		ImGui::Text("Stable"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("Whether the simulation can run at set speed at real-time."
								  " \nMeans that it is not lagging behind = frameTime<dTime ");
		if (viewer->GetDtime() > viewer->GetFrameTime())
			ImGui::TextColored({0.0f,1.0f,0.0f,1.0f}, "YES");
		else
			ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "NO");
		ImGui::NextColumn();

		ImGui::Text("Delta time"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("Basic unit of simulated time.");
		ImGui::Text("%07.4f ms", viewer->GetDtime()*1000.0); ImGui::NextColumn();

		ImGui::Text("Frame time"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("Amount of time last frame(sim + viewer) took.");
		ImGui::Text("%07.4f ms", viewer->GetFrameTime()*1000.0); ImGui::NextColumn();

		ImGui::Text("Real-time"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("How long has been the simulation running.");
		auto runTime = viewer->GetRunTime();
		auto mins = floor(runTime / 60);
		runTime -= mins * 60;
		ImGui::Text("%3.0fm %02.1fs", mins, runTime); ImGui::NextColumn();

		ImGui::Text("Sim-time"); ImGui::NextColumn();
		ImGui::TextTooltipOnHover("Amount of time simulated.");
		auto simTime = viewer->GetSimTime();
		auto years = floor(simTime / 31'536'000);
		simTime -= years * 31'536'000;
		auto days = simTime / 86'400;
		ImGui::Text("%3.0fy %03.0fd", years, days); ImGui::NextColumn();

		ImGui::Columns(1);
	}
	void GUIDrawer::ManualControls()
	{
		//If mouse will be dragged, get current offset
		if (ImGui::IsMouseClicked(0))
			offset = viewer->GetOffset();
		//Only count dragging in the center of the screen(not in side windows)
		if (ImGui::IsMouseHoveringRect({210.0f,0.0f}, {1200.0f,700.0f}, false) && ImGui::IsMouseDragging(0, 5.0f))
		{
			ImGuiIO& io = ImGui::GetIO();
			auto val = ImGui::GetMouseDragDelta();
			//Normalize values
			val.x /= io.DisplaySize.x;
			val.y /= io.DisplaySize.x;// .x is correct
			drag = {val.x * 2,-val.y * 2};
			viewer->Move(drag + offset);
		}
	}
	void GUIDrawer::ZoomControl()
	{
		float currentZoom = static_cast<float>(viewer->ScaleFactor());

		ImGuiIO& io = ImGui::GetIO();
		//Zoom based on mouse scrolling this frame
		//Also zooms faster for already zoomed screen
		currentZoom += static_cast<float>(io.MouseWheel*viewer->GetDtime()*std::max(currentZoom, 0.0f));
		//Clamp between 0 and 1000.0f, this works for AU units
		currentZoom = std::min(1000.0f, std::max(0.0f, currentZoom));

		//Zoom to center of the screen
		// - If we are not following a unit, screen is probably not centered,
		//   therefore we want to zoom to current screen center, not 0,0 coordinate.
		// - This progresivelly corrects offset to maintain same screen center regardless of zoom
		if (!follow)
		{
			auto tmpOffset = viewer->GetOffset();
			viewer->Move(tmpOffset*(currentZoom / viewer->ScaleFactor()));
		}
		viewer->ScaleFactor(currentZoom);
	}
	void GUIDrawer::Following()
	{
		// Move to this position to move followed Unit to screen's
		viewer->Move(-1.0*viewer->ScaleFactor()*(*simData)[8].pos);

		if (ImGui::Button("Cancel"))//Revert to manual controls
		{
			follow = false;
			viewer->Move({});//Centers around the 0,0
			viewer->ResetZoom(*simData);//Zooms to see whole solar system
		}
	}
	void GUIDrawer::OfferFollow()
	{
		ImGui::Checkbox("Follow Mars", &follow);
	}
}
