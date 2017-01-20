#include "GUIDrawer.h"

#include <algorithm>
#include <iostream>
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer.h"

#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
#include "GUI/UnitsProperties.h"
namespace solar
{
	namespace drawers
	{
		GUIDrawer::GUIDrawer(IMGuiViewer * parent, simData_t* data) :
			viewer(parent), follow(false), simData(data)
		{
			assert(viewer);
			assert(simData);
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
			gui::SimProperties(*viewer);
			gui::Visuals(*viewer->GetTrailsDrawer(), *simData);

		}
		void GUIDrawer::ControlsWin()
		{

			ImGui::SetNextWindowPos(ImVec2(10, 310), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(250, 390), ImGuiSetCond_Once);
			if (ImGui::Begin("Units' properties", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				unitsViewer(*simData, viewer);
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

	}
}
