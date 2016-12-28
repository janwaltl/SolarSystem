#include "GUIDrawer.h"

#include <algorithm>
#include <iostream>
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer.h"

namespace solar
{
	namespace drawers
	{
		GUIDrawer::GUIDrawer(IMGuiViewer * parent, simData_t* data) :
			Drawer(parent), follow(false), tempRawSpeed(1), tempDTSpeed(1), simData(data), selectedUnit(0)
		{
			assert(viewer);
		}
		void GUIDrawer::Draw()
		{

			//Testing transparent window
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(150, 400), ImGuiSetCond_Once);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f,0.0f,0.0f,0.0f});
			ImGui::Begin("Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
			ImGui::Text("%.6f", length((*simData)[3].pos - (*simData)[0].pos));
			ImGui::End();
			ImGui::PopStyleColor();

			ViewControl();

			ControlsWin();
			UnitTrailsWin();
		}
		void GUIDrawer::ControlsWin()
		{

			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(250, 690), ImGuiSetCond_Once);
			if (ImGui::Begin("Simulation's controls", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				SimControls();
				ImGui::Separator();
				UnitsViewer();
				ImGui::End();
			}
		}
		void GUIDrawer::SimControls()
		{
			ImGui::Text("Speed controls:");
			SpeedControl();
			ImGui::Separator();
			ImGui::Text("Metrics:");
			SimMetrics();
		}
		void GUIDrawer::SpeedControl()
		{
			//Pause/Resume buttons

			ImGui::Text("Simulation is "); ImGui::SameLine();
			if (viewer->IsRunning())
			{
				if (ImGui::StateSmallButtonWithTooltip("Running", "Pause", false))
					viewer->PauseSimulation();
			}
			else
				if (ImGui::StateSmallButtonWithTooltip("Paused", "Resume", true))
					viewer->ResumeSimulation();

			//Speed control

			ImGui::Text("Raw speed: ");
			ImGui::TextTooltipOnHover("Controls speed via rawMultiplier");	ImGui::SameLine();

			if (ImGui::SmallButton("Change##RAW"))
			{
				ImGui::OpenPopup(editRawMultPopUp);
				tempRawSpeed = viewer->GetRawMultiplier();
			}

			EditRawMultPopUp();//PopUp, because of ImGui immediate architecture must be called even if not opened

			ImGui::Text("DT speed: ");
			ImGui::TextTooltipOnHover("Controls speed via DTMultiplier");	ImGui::SameLine();

			if (ImGui::SmallButton("Change##DT"))
			{
				ImGui::OpenPopup(editDTMultPopUp);
				tempDTSpeed = viewer->GetDTMultiplier();
			}

			EditDTMultPopUp();//PopUp, because of ImGui immediate architecture must be called even if not opened

			if (viewer->IsPaused())//Stepping simulation
			{

				ImGui::Text("Stepping:");
				ImGui::TextTooltipOnHover("Steps simulation.");	ImGui::SameLine();

				if (ImGui::SmallButton("->##Step"))
					viewer->StepSimulation();
				ImGui::TextTooltipOnHover("Makes one step forwards");	ImGui::SameLine();
			}
			ImGui::NewLine();
		}

		void GUIDrawer::EditRawMultPopUp()
		{
			if (ImGui::BeginPopup(editRawMultPopUp))
			{
				ImGui::InputInt("", &tempRawSpeed, 1, 1000);
				if (tempRawSpeed < 1)
					tempRawSpeed = 1;
				if (static_cast<size_t>(tempRawSpeed) > viewer->GetRawMultiplier())
					ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "High values may result higher frameTimes "
									   "and higher CPU costs which might result in loss of responsivness(FPS&UPS).");

				if (ImGui::SmallButton("Set"))
				{
					viewer->SetRawMultiplier(static_cast<size_t>(tempRawSpeed));
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}

		void GUIDrawer::EditDTMultPopUp()
		{
			if (ImGui::BeginPopup(editDTMultPopUp))
			{
				ImGui::InputInt("", &tempDTSpeed, 100, 10'000);
				if (tempDTSpeed < 1)
					tempDTSpeed = 1;

				if (static_cast<size_t>(tempDTSpeed) > viewer->GetDTMultiplier())
					ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "High values may result in lost of precision "
									   "and stability of simulated system.");
				if (ImGui::SmallButton("Set"))
				{
					viewer->SetDTMultiplier(static_cast<size_t>(tempDTSpeed));
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}

		void GUIDrawer::ViewControl()
		{
			//Always allow zoom by mouse
			ZoomControl();
			//If we are not following, give translating control to mouse, else we give control to Following()
			if (!follow)
			{
				ManualControls();
			}
			else
				Following();
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
		void GUIDrawer::UnitsViewer()
		{
			//Shows list of all units and offers some things to do with them
			ImGui::Text("Show info about selected unit:");

			//Returns char* string representing name of the selected Unit by its index in simData vector
			auto unitNameGetter = [](void* data, int index, const char** result)->bool {
				//simData can also be captured, but might as well use this intended argument
				auto simData = reinterpret_cast<simData_t*>(data);
				*result = simData->at(index).name.c_str();//does not throw, correct index is ensured by ImGui::Combo call
				return true;
			};
			//0 = center system, index>0 gets simData(index-1) unit's name
			auto refSystemGetter = [](void* data, int index, const char** result)->bool {
				//simData can also be captured, but might as well use this intended argument
				auto simData = reinterpret_cast<simData_t*>(data);
				const static char* centerSystem = "(0,0) coordinates";
				if (index == 0)
					*result = centerSystem;
				else
					*result = simData->at(index - 1).name.c_str();//does not throw, correct index is ensured by ImGui::Combo call

				return true;
			};

			ImGui::Combo("##SelectedUnit", &selectedUnit, unitNameGetter, simData, simData->size(), 5);
			ImGui::Text("Reference system:");
			ImGui::TextTooltipOnHover("Physical units are relative with respect to selected reference system.");
			//Zero index is reserved for 0,0 coordinates, so there is one more than data->size()
			ImGui::Combo("##SelectedRefSystem", &selectedRefSystem, refSystemGetter, simData, simData->size() + 1, 5);

			ImGui::Text("Properties:");
			auto& refUnit = selectedRefSystem > 0 ? simData->at(selectedRefSystem - 1) : centerSystem;
			if (0 <= selectedUnit && selectedUnit < simData->size())
			{
				auto& unit = (*simData)[selectedUnit];
				ImGui::Columns(2, "##UnitsCol", false);
				ImGui::SetColumnOffset(1, 80.0f);
				ImGui::Text("Position:"); ImGui::NextColumn();
				ImGui::Text("X %2.6f AU", unit.pos.X() - refUnit.pos.X()); ImGui::NextColumn();
				if (ImGui::SmallButton(!follow ? "Follow" : "Cancel")) follow = !follow;
				ImGui::TextTooltipOnHover(!follow ? "Follow selected unit around" : "Stop following selected unit");
				ImGui::NextColumn();
				ImGui::Text("Y %2.6f AU", unit.pos.Y() - refUnit.pos.Y()); ImGui::NextColumn();

				ImGui::Text("Distance:"); ImGui::NextColumn();
				ImGui::TextTooltipOnHover("Distance from center of reference system.");
				ImGui::Text(" %2.6f AU", length(unit.pos - refUnit.pos));
				ImGui::NextColumn();

				ImGui::Text("Velocity:"); ImGui::NextColumn();
				//Convert to km/s
				ImGui::Text("X %+2.6f km/s", (unit.vel.X() - refUnit.vel.X())*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::Text("Y %+2.6f km/s", (unit.vel.Y() - refUnit.vel.Y())*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::NextColumn();

				ImGui::Text("Speed:"); ImGui::NextColumn();
				ImGui::Text(" %2.6f km/s", length(unit.vel - refUnit.vel)*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::NextColumn();

				ImGui::Text("Mass:"); ImGui::NextColumn();
				ImGui::Text(" %1.4e kg", unit.mass * physicsUnits::SMtoKG); ImGui::NextColumn();
				ImGui::Columns(1);
			}
			else
				ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "No unit is selected.");


		}
		void GUIDrawer::UnitTrailsWin()
		{
			ImGui::SetNextWindowPos(ImVec2(1000, 10), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(200, 690), ImGuiSetCond_Once);
			if (ImGui::Begin("Visuals", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				if (ImGui::CollapsingHeader("Line Trails:", nullptr, false, true))
				{
					LineTrailsSwitching();

					if (ImGui::SmallButton("Enable all"))
						this->viewer->GetTrailsDrawer()->SwitchAll(true);
					//ImGui::SameLine();
					if (ImGui::SmallButton("Disable all"))
						this->viewer->GetTrailsDrawer()->SwitchAll(false);

					if (ImGui::SmallButton("Reset all"))
						this->viewer->GetTrailsDrawer()->ClearAll();
					ImGui::TextTooltipOnHover("Deletes all drawn trails.");
				}

				ImGui::End();
			}
		}
		void GUIDrawer::LineTrailsSwitching()
		{
			if (ImGui::BeginChild("Line Trails", ImVec2(0, 300), false, ImGuiWindowFlags_AlwaysUseWindowPadding))
			{
				for (size_t i = 0; i < simData->size(); ++i)
				{
					bool checked = this->viewer->GetTrailsDrawer()->IsTrailEnabled(i);
					if (ImGui::Checkbox((*simData)[i].name.c_str(), &checked))
						this->viewer->GetTrailsDrawer()->SwitchTrail(i, checked);
				}
				ImGui::EndChild();
			}
		}
		void GUIDrawer::ManualControls()
		{
			//If mouse will be dragged, get current offset
			if (ImGui::IsMouseClicked(0))
				offset = viewer->GetOffset();
			//Only count dragging in the center of the screen(not in side windows)
			if (ImGui::IsMouseHoveringRect({260.0f,0.0f}, {1000.0f,700.0f}, false) && ImGui::IsMouseDragging(0, 5.0f))
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
			//Allow scrolling only in center region, not in the left window
			//Also zooms faster for already zoomed screen
			if (ImGui::IsMouseHoveringRect({260.0f,0.0f}, {1000.0f,700.0f}, false))
				currentZoom += static_cast<float>(5 * io.MouseWheel*viewer->GetFrameTime()*std::max(currentZoom, 0.0f));

			//Clamp between 0 and 1000.0f, this works for AU units
			currentZoom = std::min(1000.0f, std::max(0.001f, currentZoom));

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
			//Follow can begin only by clicking follow button with selected unit
			assert(simData->size() > selectedUnit && selectedUnit >= 0);
			// Move to this position to move followed Unit to screen's center
			viewer->Move(-1.0*viewer->ScaleFactor()*(*simData)[selectedUnit].pos);
		}

	}
}
