#include "VisualPreferences.h"

#include "Source/Viewers/IMGuiViewer/Drawers/SceneDrawer.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/Drawers/Scene/LineTrailsDrawer.h"

namespace solar
{
	namespace gui
	{
		namespace
		{
			bool UnitNameGetter(void * data, int index, const char ** result)
			{
				static const char* none = "<Free>";
				if (index == 0)
					*result = none;
				else
				{
					auto& simData = *reinterpret_cast<SimData*>(data);
					*result = simData[index - 1].name.c_str();// correct index is ensured by ImGui::Combo call
				}
				return true;
			}
		}
		void Visuals(drawers::LineTrailsDrawer & lineTrails, const SimData & data, size_t w, size_t h)
		{

			ImGui::SetNextWindowPos(ImVec2(float(w - 250.0f), 10.0f), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(250.0f, std::min(400.0f, (float)h)), ImGuiSetCond_Once);
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
		void LineTrailsBoxes(drawers::LineTrailsDrawer & lineTrails, const SimData & data)
		{
			///Assert that there are as many lineTrails as units
			if (ImGui::BeginChild("Line Trails", ImVec2(0, 300), false, ImGuiWindowFlags_AlwaysUseWindowPadding))
			{
				for (size_t i = 0; i < data->size(); ++i)
				{
					bool checked = lineTrails.IsTrailEnabled(i);
					if (ImGui::Checkbox(data.Get()[i].name.c_str(), &checked))
						lineTrails.SwitchTrail(i, checked);
				}
				ImGui::EndChild();
			}
		}

		void VisualPreferences::Draw(SimData & data, drawers::SceneDrawer & scene)
		{
			auto menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 400, menuBarHeight), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiSetCond_Once);
			if (ImGui::Begin("Visuals", NULL))
			{
				if (ImGui::CollapsingHeader("Line Trails:", nullptr, false, false))
				{
					LineTrailsControls(scene.GetLineTrails());
					LineTrailsBoxes(scene.GetLineTrails(), data);
				}
				if (ImGui::CollapsingHeader("Camera Controls", ImGuiTreeNodeFlags_DefaultOpen))
				{
					CameraControls(data, scene);
				}
			}
			ImGui::End();
		}

		void VisualPreferences::CameraControls(SimData & data, drawers::SceneDrawer & scene)
		{
			ImGui::Text("Camera's type: "); ImGui::SameLine();
			if (ImGui::Combo("##CamsType", &combo.camType, "Perspective\0Orthographic\0"))
			{
				switch (combo.camType)
				{
				case 0:
					scene.SetActiveCam(scene.perspective);
					break;
				case 1:
					scene.SetActiveCam(scene.ortho);
					break;
				}
			}
			ImGui::Text("Camera's target:"); ImGui::SameLine();
			if (ImGui::Combo("##CamTarget", &combo.camTarget, UnitNameGetter, &data, data->size() + 1))
				scene.GetActiveCam().FollowObject(combo.camTarget - 1);
			ImGui::TextTooltipOnHover("Locks camera's orientation to always point to targeted object.\n"
									  "Only zooming and trackball works while set.");
			//ImGui::Text("Camera's position:"); ImGui::SameLine();
			//ImGui::Combo("##CamPosition", &combo.camPos, UnitNameGetter, &data, data->size() + 1);
			//ImGui::TextTooltipOnHover("Sets and locks camera's position to be same as chosen object's.\n"
			//						  "Only 1st-person camera works while set.");

			ImGui::Text("Controlled by Mouse: ");
			ImGui::Text("Left button:  Trackball rotation around camera's target.\n"
						"Right button: Move to sides.\n"
						"ALT+LB:       1st-person camera.\n"
						"Wheel:        Zoom to target.\n"
						"ALT+Wheel:    Move forwards/backwards.\n");
		}
	}
}
