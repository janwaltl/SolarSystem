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
			camControls.MouseControls(scene.GetCam(), data);
			TopMenuBar(data, viewer, scene, w, h);
			auto context = ImGui::GetCurrentContext();
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
					ImU32 smallCol = ImGui::ColorConvertFloat4ToU32(ImVec4 {0.06f, 0.14f, 0.31f,1.0f});
					ImU32 bigCol = ImGui::ColorConvertFloat4ToU32(ImVec4 {0.06f, 0.24f, 0.71f,1.0f});
					auto gridPos = w - grid - 2 * context->Style.FramePadding.x;
					ImGui::SameLine();
					draw->AddLine(ImVec2(gridPos - 200.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 5.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 5.0f),
								  bigCol, 2.5f);

					draw->AddLine(ImVec2(gridPos - 200.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 10.0f),
								  ImVec2(gridPos - 200.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  bigCol, 2.5f);
					draw->AddLine(ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 10.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  bigCol, 2.5f);

					draw->AddLine(ImVec2(gridPos - 100.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 5.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 5.0f),
								  smallCol, 2.5f);
					draw->AddLine(ImVec2(gridPos - 100.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  ImVec2(gridPos - 100.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 10.0f),
								  smallCol, 2.5f);
					draw->AddLine(ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
								  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 10.0f),
								  smallCol, 2.5f);

					ImGui::SameLine(gridPos); ImGui::Text("10 kilometres/1 kilometre");
					grid = ImGui::GetItemRectSize().x;
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();


		}
		void GUIDrawer::TopMenuBar(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h)
		{
			//To reflect changes from the outside code
			switches.pause.selected = viewer.IsPaused() ? true : false;
			switches.play.selected = viewer.IsRunning() ? true : false;

			auto context = ImGui::GetCurrentContext();
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowContentSize(ImVec2(float(w), 0));
			static float pause = 0.0f, prefs = 0.0f;
			if (ImGui::Begin("TopMenuBar", nullptr, ImVec2(float(w), 0), 0.0f, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar |
							 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				if (ImGui::BeginMenuBar())
				{
					auto draw = ImGui::GetWindowDrawList();
					ImGui::AlignFirstTextHeightToWidgets();
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					buttons.graphs.Draw("Graphs");
					ImGui::PopStyleColor();

					if (buttons.graphs.selected)
						graphs(data, viewer.GetRunTime(), viewer.GetSimTime());
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					buttons.unitsProps.Draw("List of objects");
					ImGui::PopStyleColor();
					if (buttons.unitsProps.selected)
						unitsProps(data);
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					buttons.simControls.Draw("Simulation's Details");
					ImGui::PopStyleColor();
					if (buttons.simControls.selected)gui::SimProperties(viewer);

					ImGui::SameLine(w / 2.0f - pause / 2.0f);//Centered
					ImGui::BeginGroup();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					TopButtons(viewer, draw);
					ImGui::EndGroup();
					pause = ImGui::GetItemRectSize().x;
					ImGui::SameLine(w - prefs - context->Style.FramePadding.x);
					ImGui::BeginGroup();
					TopRightSwitches(viewer, draw);
					ImGui::SameLine(); ImGui::Button("Hide UI");//Grid on/off
					ImGui::SameLine();
					buttons.visuals.Draw("Visual Preferences");
					ImGui::PopStyleColor();
					if (buttons.visuals.selected)
						camControls(scene.GetCam(), data);
					ImGui::EndGroup();
					prefs = ImGui::GetItemRectSize().x;
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
		}
		void GUIDrawer::TopRightSwitches(solar::Viewer & viewer, ImDrawList * draw)
		{
			auto style = ImGui::GetStyle();
			auto buttonSize = style.FramePadding * 2.0f + ImGui::CalcTextSize("XX");
			auto textCol = ImGui::GetColorU32(ImGuiCol_Text);
			ImVec2 cursorPos;
			//RealPlanetScale Button
			cursorPos = ImGui::GetCursorScreenPos();
			switches.planetScale.DrawBlank(buttonSize);
			ImGui::SameLine();
			draw->AddCircle(cursorPos + ImVec2(buttonSize.x*0.4f, buttonSize.y*0.5f), buttonSize.x*0.3f, textCol, 12, 2.0f);
			draw->AddCircleFilled(cursorPos + buttonSize*0.75f, buttonSize.x*0.4f - style.FramePadding.x, textCol, 12);
			//LineTrails button
			cursorPos = ImGui::GetCursorScreenPos();
			switches.lineTrails.DrawBlank(buttonSize);
			ImGui::SameLine();
			draw->PathArcTo(cursorPos + style.FramePadding, buttonSize.x*0.5f, 0, 3.14f / 2);
			draw->PathStroke(textCol, false, 2.0f);
			draw->AddCircleFilled(cursorPos + buttonSize*0.6f, buttonSize.x*0.15f, textCol);
			//Grid button
			cursorPos = ImGui::GetCursorScreenPos();
			switches.grid.DrawBlank(buttonSize);
			ImGui::SameLine();
			for (int i = 1; i <= 3; ++i)
			{
				draw->AddLine(cursorPos + ImVec2(buttonSize.x*0.25f*i, style.FramePadding.y),
							  cursorPos + ImVec2(buttonSize.x*0.25f*i, buttonSize.y - style.FramePadding.y),
							  textCol, 1.0f);
				draw->AddLine(cursorPos + ImVec2(style.FramePadding.x, buttonSize.x*0.25f*i),
							  cursorPos + ImVec2(buttonSize.x - style.FramePadding.x, buttonSize.x*0.25f*i),
							  textCol, 1.0f);
			}
		}
		void GUIDrawer::TopButtons(solar::Viewer & viewer, ImDrawList * draw)
		{
			auto style = ImGui::GetStyle();
			auto buttonSize = style.FramePadding * 2.0f + ImGui::CalcTextSize("XX");
			auto textCol = ImGui::GetColorU32(ImGuiCol_Text);
			ImVec2 cursorPos;
			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##<<Raw", buttonSize))
			{

			}
			ImGui::SameLine();
			DrawSlowerTriangles(draw, cursorPos, buttonSize, style, textCol);
			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##<<DT", buttonSize))
			{

			}
			ImGui::SameLine();
			DrawSlowerTriangles(draw, cursorPos, buttonSize, style, textCol);

			//Pause Button
			cursorPos = ImGui::GetCursorScreenPos();
			if (switches.pause.DrawBlank(buttonSize) && viewer.IsRunning())
			{
				viewer.PauseSimulation();
				switches.play.selected = false;
			}
			ImGui::SameLine();
			//Draw pause paralel lines
			draw->AddLine(ImVec2(cursorPos.x + buttonSize.x*0.4f, cursorPos.y + style.FramePadding.y),
						  ImVec2(cursorPos.x + buttonSize.x*0.4f, cursorPos.y + buttonSize.y - style.FramePadding.x),
						  textCol, 2.0f);
			draw->AddLine(ImVec2(cursorPos.x + buttonSize.x*0.6f, cursorPos.y + style.FramePadding.y),
						  ImVec2(cursorPos.x + buttonSize.x*0.6f, cursorPos.y + buttonSize.y - style.FramePadding.x),
						  textCol, 2.0f);
			//Play button
			cursorPos = ImGui::GetCursorScreenPos();
			if (switches.play.DrawBlank(buttonSize) && viewer.IsPaused())
			{
				viewer.ResumeSimulation();
				switches.pause.selected = false;
			}
			ImGui::SameLine();
			//Render play button at the centre of the button
			draw->AddTriangleFilled(ImVec2(cursorPos.x + style.FramePadding.x, cursorPos.y + style.FramePadding.y),
									ImVec2(cursorPos.x + style.FramePadding.x, cursorPos.y + buttonSize.y - style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x - style.FramePadding.x, cursorPos.y + buttonSize.y*0.5f), textCol);
			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##>>DT", buttonSize))
			{

			}
			ImGui::SameLine();
			DrawFasterTriangles(draw, cursorPos, style, buttonSize, textCol);
			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##>>Raw", buttonSize))
			{

			}
			ImGui::SameLine();
			DrawFasterTriangles(draw, cursorPos, style, buttonSize, textCol);
		}
		void GUIDrawer::DrawFasterTriangles(ImDrawList * draw, ImVec2 &cursorPos, ImGuiStyle &style, ImVec2 &buttonSize, const ImU32 &textCol)
		{
			draw->AddTriangleFilled(ImVec2(cursorPos.x + style.FramePadding.x, cursorPos.y + style.FramePadding.y),
									ImVec2(cursorPos.x + style.FramePadding.x, cursorPos.y + buttonSize.y - style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x*0.5f, cursorPos.y + buttonSize.y*0.5f), textCol);
			draw->AddTriangleFilled(ImVec2(cursorPos.x + buttonSize.x*0.5f, cursorPos.y + style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x*0.5f, cursorPos.y + buttonSize.y - style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x - style.FramePadding.x, cursorPos.y + buttonSize.y*0.5f), textCol);
		}
		void GUIDrawer::DrawSlowerTriangles(ImDrawList * draw, ImVec2 &cursorPos, ImVec2 &buttonSize, ImGuiStyle &style, const ImU32 &textCol)
		{
			draw->AddTriangleFilled(ImVec2(cursorPos.x + buttonSize.x*0.5f, cursorPos.y + style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x*0.5f, cursorPos.y + buttonSize.y - style.FramePadding.y),
									ImVec2(cursorPos.x + style.FramePadding.x, cursorPos.y + buttonSize.y*0.5f), textCol);
			draw->AddTriangleFilled(ImVec2(cursorPos.x + buttonSize.x - style.FramePadding.x, cursorPos.y + style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x - style.FramePadding.x, cursorPos.y + buttonSize.y - style.FramePadding.y),
									ImVec2(cursorPos.x + buttonSize.x*0.5f, cursorPos.y + buttonSize.y*0.5f), textCol);
		}
	}
}
