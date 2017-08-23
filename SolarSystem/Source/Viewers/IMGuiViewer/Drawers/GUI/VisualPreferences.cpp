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
			bool ObjectNameGetter(void * data, int index, const char ** result)
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
			constexpr Vec2f winSize {400.0f,500.0f};
			constexpr float popUpSize = 400.0f;
			const char* lineTrailsPopUp = "Line Trails";
		}
		void VisualPreferences::LineTrailsControls(drawers::LineTrailsDrawer & lineTrails)
		{

			if (ImGui::Button("Enable "))
				lineTrails.SwitchAll(true);
			ImGui::TextTooltipOnHover("Enable drawing of all trails.");
			ImGui::SameLine();
			if (ImGui::Button("Disable"))
				lineTrails.SwitchAll(false);
			ImGui::TextTooltipOnHover("Disables drawing of all trails.");
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				lineTrails.ClearAll();
			ImGui::TextTooltipOnHover("Deletes all drawn trails.");
			ImGui::SameLine();
			if (ImGui::Button("Switch Individual trails"))
				ImGui::OpenPopup(lineTrailsPopUp);
		}
		void VisualPreferences::LineTrailsPopUp(drawers::LineTrailsDrawer & lineTrails, const SimData & data)
		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(popUpSize, 0), ImVec2(popUpSize, popUpSize));
			if (ImGui::BeginPopupModal(lineTrailsPopUp, nullptr))
			{
				ImGui::Columns(3);
				for (size_t i = 0; i < data->size(); ++i)
				{
					bool checked = lineTrails.IsTrailEnabled(i);
					if (ImGui::Checkbox(data.Get()[i].name.c_str(), &checked))
						lineTrails.SwitchTrail(i, checked);
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}

		void VisualPreferences::Draw(SimData & data, drawers::SceneDrawer & scene)
		{
			auto menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 400, menuBarHeight), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Once);
			if (ImGui::Begin("Visuals", NULL))
			{
				if (ImGui::CollapsingHeader("Line Trails:", ImGuiTreeNodeFlags_DefaultOpen))
				{
					LineTrailsControls(scene.GetLineTrails());
					LineTrailsPopUp(scene.GetLineTrails(), data);
				}
				if (ImGui::CollapsingHeader("Camera Controls", ImGuiTreeNodeFlags_DefaultOpen))
				{
					CameraControls(data, scene);
				}
				if (ImGui::CollapsingHeader("Grid settings", ImGuiTreeNodeFlags_DefaultOpen))
				{
					constexpr float spacing = 120.0f;
					ImGui::AlignFirstTextHeightToWidgets();
					ImGui::Text("Grid plane:");
					ImGui::TextTooltipOnHover("Plane in which will the grid be drawn.");
					ImGui::SameLine(spacing);
					if (ImGui::Combo("##GridPlane", &combo.gridPlane, "XY\0YZ\0XZ\0"))
						//IMPROVE Cast to enum
						// - but thats dependent on enum order, so if/else used for now
						scene.SetGridPlane(combo.gridPlane == 0 ? drawers::SceneDrawer::plane::XY : (combo.gridPlane == 1 ? drawers::SceneDrawer::plane::YZ : drawers::SceneDrawer::plane::XZ));

					gridOffset = scene.GetGridOffset();
					ImGui::Text("Grid offset:");
					ImGui::SameLine(spacing);
					if (ImGui::SliderFloat("##Offset", &gridOffset, -1.0f, 1.0f))
						scene.SetGridOffset(gridOffset);

					checkBox.pinheads = scene.ArePinHeadsEnabled();
					if (ImGui::Checkbox("Show pin heads", &checkBox.pinheads))
						scene.SwitchGrid(scene.IsGridEnabled(), checkBox.pinheads);
					ImGui::TextTooltipOnHover("Draws vertical line between object and its projection onto the grid.");
				}
				if (ImGui::CollapsingHeader("Planets", ImGuiTreeNodeFlags_DefaultOpen))
				{
					minScreenRadius = scene.GetSimDataDrawer().GetMinScreenRadius();
					ImGui::SliderFloat("Min radius", &minScreenRadius, 0.0f, 0.5f);
					scene.GetSimDataDrawer().SetMinScreenRadius(minScreenRadius);
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
			if (ImGui::Combo("##CamTarget", &combo.camTarget, ObjectNameGetter, &data, data->size() + 1))
				scene.GetActiveCam().FollowObject(combo.camTarget - 1);
			ImGui::TextTooltipOnHover("Locks camera's orientation to always point to targeted object.\n"
									  "Only zooming and trackball works while set.");
			//TODO when camera can be positioned
			//ImGui::Text("Camera's position:"); ImGui::SameLine();
			//ImGui::Combo("##CamPosition", &combo.camPos, ObjectNameGetter, &data, data->size() + 1);
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
