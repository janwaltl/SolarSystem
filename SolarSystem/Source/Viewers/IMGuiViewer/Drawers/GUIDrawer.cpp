#include "GUIDrawer.h"

#include <algorithm>
#include <iostream>
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer.h"

#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
namespace solar
{
	namespace drawers
	{
		GUIDrawer::GUIDrawer(IMGuiViewer * parent, simData_t* data) :
			viewer(parent), follow(false), tempRawSpeed(1), tempDTSpeed(1), simData(data), selectedUnit(0)
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
			gui::Visuals(*viewer->GetTrailsDrawer(), *simData);

		}
		void GUIDrawer::ControlsWin()
		{
			gui::SimProperties(*viewer);

			ImGui::SetNextWindowPos(ImVec2(10, 310), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(250, 390), ImGuiSetCond_Once);
			if (ImGui::Begin("Units' properties", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				UnitsViewer();
				ImGui::End();
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
				ImGui::Text("X %2.6f AU", unit.pos.x - refUnit.pos.x); ImGui::NextColumn();
				if (ImGui::SmallButton(!follow ? "Follow" : "Cancel")) follow = !follow;
				ImGui::TextTooltipOnHover(!follow ? "Follow selected unit around" : "Stop following selected unit");
				ImGui::NextColumn();
				ImGui::Text("Y %2.6f AU", unit.pos.y - refUnit.pos.y); ImGui::NextColumn();

				ImGui::Text("Distance:"); ImGui::NextColumn();
				ImGui::TextTooltipOnHover("Distance from center of reference system.");
				ImGui::Text(" %2.6f AU", length(unit.pos - refUnit.pos));
				ImGui::NextColumn();

				ImGui::Text("Velocity:"); ImGui::NextColumn();
				//Convert to km/s
				ImGui::Text("X %+2.6f km/s", (unit.vel.x - refUnit.vel.x)*physicsUnits::AUpYtoMpS / 1000.0);
				ImGui::Text("Y %+2.6f km/s", (unit.vel.y - refUnit.vel.y)*physicsUnits::AUpYtoMpS / 1000.0);
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
				drag = Vec2(val.x * 2, -val.y * 2);
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
