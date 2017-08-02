#include "GUIDrawer.h"


#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
#include "GUI/CameraControls.h"
#include "SceneDrawer.h"


namespace solar
{
	namespace drawers
	{
		void GUIDrawer::Draw(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h)
		{
			camControls(scene.GetCam(), data);
			//unitsProps(data);
			//graphs(data, viewer.GetRunTime(), viewer.GetSimTime());
			gui::SimProperties(viewer);
			//gui::Visuals(scene.GetLineTrails(), data, w, h);

			auto context = ImGui::GetCurrentContext();
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowContentSize(ImVec2(float(w), 0));
			static float pause = 0.0f, prefs = 0.0f;
			if (ImGui::Begin("TopMenuBar", nullptr, ImVec2(float(w), 0), 0.0f,
							 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					ImGui::AlignFirstTextHeightToWidgets();
					ImGui::SameLine(); ImGui::Button("Graphs");
					ImGui::SameLine(); ImGui::Button("List of objects");
					ImGui::SameLine(); ImGui::Button("Simulation's Details");
					//auto width = ImGui::GetCursorPosX();
					ImGui::SameLine(w / 2.0 - pause / 2.0f);
					ImGui::BeginGroup();
					ImGui::Button("<<");
					ImGui::SameLine(); ImGui::Button("<<");
					ImGui::SameLine(); ImGui::Button("||");
					ImGui::SameLine(); ImGui::Button("|>");
					ImGui::SameLine(); ImGui::Button(">>");
					ImGui::SameLine(); ImGui::Button(">>");
					ImGui::EndGroup();
					pause = ImGui::GetItemRectSize().x;
					ImGui::SameLine(w - prefs - context->Style.FramePadding.x);
					ImGui::BeginGroup();
					ImGui::Button("PS");//Real planet scale
					ImGui::SameLine(); ImGui::Button("LT");//LineTrails on/off
					ImGui::SameLine(); ImGui::Button("GR");//Grid on/off
					ImGui::SameLine(); ImGui::Button("Hide UI");//Grid on/off
					ImGui::SameLine(); ImGui::Button("Visual Preferences");
					ImGui::EndGroup();
					prefs = ImGui::GetItemRectSize().x;
					ImGui::PopStyleColor();
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
			static float speed = 0.0f, grid = 0.0f;
			//Calculated same way as in Imgui's window calculations - function MenuBarHeight() in imgui_internal.h
			auto menuBarHeight = context->FontSize + context->Style.FramePadding.y * 2.0f;
			ImGui::SetNextWindowPos(ImVec2(0, float(h) - menuBarHeight));
			ImGui::SetNextWindowContentSize(ImVec2(float(w), 0));
			if (ImGui::Begin("BottomMenuBar", nullptr, ImVec2(float(w), 0), 0.0f,
							 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::AlignFirstTextHeightToWidgets();
					ImGui::Text("Simulated time: 10years, 320days, 18hours, 45minutes, 10seconds");
					ImGui::SameLine(w / 2.0f - speed / 2.0f); ImGui::Text("Speed: 12345678912345x");
					ImGui::TextTooltipOnHover("fsafa");
					speed = ImGui::GetItemRectSize().x;
					auto draw = ImGui::GetWindowDrawList();

					auto gridPos = w - grid - 2 * context->Style.FramePadding.x;
					ImGui::SameLine();
					draw->AddLine(ImVec2(gridPos - 200.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 5.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 5.0f),
								  ImGui::GetColorU32(ImGuiCol_ButtonActive), 2.5f);

					draw->AddLine(ImVec2(gridPos - 200.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 10.0f),
								  ImVec2(gridPos - 200.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  ImGui::GetColorU32(ImGuiCol_ButtonActive), 2.5f);
					draw->AddLine(ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 10.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  ImGui::GetColorU32(ImGuiCol_ButtonActive), 2.5f);

					draw->AddLine(ImVec2(gridPos - 20.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 5.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 5.0f),
								  ImGui::GetColorU32(ImGuiCol_Button), 2.5f);
					draw->AddLine(ImVec2(gridPos - 20.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  ImVec2(gridPos - 20.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 10.0f),
								  ImGui::GetColorU32(ImGuiCol_Button), 2.5f);
					draw->AddLine(ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 10.0f),
								  ImGui::GetColorU32(ImGuiCol_Button), 2.5f);

					ImGui::SameLine(gridPos); ImGui::Text("10 kilometres/1 kilometre");
					grid = ImGui::GetItemRectSize().x;
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();


		}
	}
}
