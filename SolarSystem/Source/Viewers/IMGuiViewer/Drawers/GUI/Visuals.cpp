#include "Visuals.h"


#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/Drawers/LineTrailsDrawer.h"

namespace solar
{
	namespace gui
	{
		void Visuals(drawers::LineTrailsDrawer & lineTrails, const simData_t & data, size_t w, size_t h)
		{

			ImGui::SetNextWindowPos(ImVec2(w - 250, 10), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(250, std::min(400, (int)h)), ImGuiSetCond_Once);
			if (ImGui::Begin("Visuals", NULL))
			{
				if (ImGui::CollapsingHeader("Line Trails:", nullptr, false, false))
				{
					LineTrailsControls(lineTrails);
					LineTrailsBoxes(lineTrails, data);
				}
			}
			ImGui::End();
		}
		void LineTrailsControls(drawers::LineTrailsDrawer & lineTrails)
		{
			ImGui::Text("LineTrails' Controls:");

			if (ImGui::SmallButton("Enable "))
				lineTrails.SwitchAll(true);
			ImGui::TextTooltipOnHover("Enable drawing of all trails.");
			ImGui::SameLine();
			if (ImGui::SmallButton("Disable"))
				lineTrails.SwitchAll(false);
			ImGui::TextTooltipOnHover("Disables drawing of all trails.");
			ImGui::SameLine();
			if (ImGui::SmallButton("Reset"))
				lineTrails.ClearAll();
			ImGui::TextTooltipOnHover("Deletes all drawn trails.");
		}
		void LineTrailsBoxes(drawers::LineTrailsDrawer & lineTrails, const simData_t & data)
		{
			///Assert that there are as many lineTrails as units
			if (ImGui::BeginChild("Line Trails", ImVec2(0, 300), false, ImGuiWindowFlags_AlwaysUseWindowPadding))
			{
				for (size_t i = 0; i < data.size(); ++i)
				{
					bool checked = lineTrails.IsTrailEnabled(i);
					if (ImGui::Checkbox(data[i].name.c_str(), &checked))
						lineTrails.SwitchTrail(i, checked);
				}
				ImGui::EndChild();
			}
		}

	}
}
