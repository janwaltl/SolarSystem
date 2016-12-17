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
		ImGui::Text("%.6f", length(data[3].pos));
		ImGui::End();
		ImGui::PopStyleColor();

		ControlsWin();
	}
	void GUIDrawer::ControlsWin()
	{

		ImGui::SetNextWindowPos(ImVec2(1000, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiSetCond_Once);
		if (ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoCollapse |
						 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
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
	void GUIDrawer::ManualControls()
	{
		//If mouse will be dragged, get current offset
		if (ImGui::IsMouseClicked(0))
			offset = viewer->GetOffset();
		//Only count dragging in the center of the screen(not in side windows)
		if (ImGui::IsMouseHoveringRect({160.0f,0.0f}, {1000.0f,700.0f}, false) && ImGui::IsMouseDragging(0, 5.0f))
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
