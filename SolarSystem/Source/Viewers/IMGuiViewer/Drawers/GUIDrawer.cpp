#include "GUIDrawer.h"


#include "GUI/VisualPreferences.h"
#include "GUI/SimProperties.h"
#include "SceneDrawer.h"


namespace solar
{
	namespace drawers
	{
		GUIDrawer::GUIDrawer(SimData& data) :
			objectContextMenu(data)
		{
			switches.grid.selected = true;
			switches.lineTrails.selected = true;
			switches.planetScale.selected = false;
		}
		void GUIDrawer::Draw(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h)
		{
			TopMenuBar(data, viewer, scene, w, h);
			BotttomMenuBar(data, viewer, scene, w, h);
			objectContextMenu.Draw(data, scene);
		}
		void GUIDrawer::TopMenuBar(SimData& data, Viewer& viewer, SceneDrawer& scene, size_t w, size_t h)
		{
			//To reflect changes from the outside code
			switches.pause.selected = viewer.IsPaused() ? true : false;
			switches.play.selected = viewer.IsRunning() ? true : false;

			auto context = ImGui::GetCurrentContext();
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowContentSize(ImVec2(float(w), 0));
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
					ImGui::TextTooltipOnHover("Enables creating graphs visualizing various physical properties of simulated objects.");
					ImGui::PopStyleColor();

					if (buttons.graphs.selected)
						graphs(data, viewer.GetRunTime(), viewer.GetSimTime());
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					buttons.unitsProps.Draw("List of objects");
					ImGui::TextTooltipOnHover("Shows all simulated objects and their physical properties.");
					ImGui::PopStyleColor();
					if (buttons.unitsProps.selected)
						unitsProps(data);
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					buttons.simControls.Draw("Simulation's Details");
					ImGui::TextTooltipOnHover("Shows statistics about ongoing simulation.");
					ImGui::PopStyleColor();
					if (buttons.simControls.selected)gui::SimProperties(viewer);

					ImGui::SameLine(w / 2.0f - offsets.pause / 2.0f);//Centered
					ImGui::BeginGroup();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
					TopButtons(viewer, draw);
					ImGui::EndGroup();
					offsets.pause = ImGui::GetItemRectSize().x;
					ImGui::SameLine(w - offsets.prefs - context->Style.FramePadding.x);
					ImGui::BeginGroup();
					TopRightSwitches(scene, viewer, draw);
					ImGui::SameLine(); ImGui::Button("Hide UI");//Grid on/off
					ImGui::SameLine();
					buttons.visuals.Draw("Visual Preferences");
					ImGui::TextTooltipOnHover("Allows to change visual parts of the simulation.");
					ImGui::PopStyleColor();
					if (buttons.visuals.selected)
						visualPrefs.Draw(data, scene);
					ImGui::EndGroup();
					offsets.prefs = ImGui::GetItemRectSize().x;
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
		}

		void GUIDrawer::TopRightSwitches(SceneDrawer& scene, solar::Viewer & viewer, ImDrawList * draw)
		{
			auto style = ImGui::GetStyle();
			auto buttonSize = style.FramePadding * 2.0f + ImGui::CalcTextSize("XX");
			auto textCol = ImGui::GetColorU32(ImGuiCol_Text);
			ImVec2 cursorPos;
			//RealPlanetScale Button
			cursorPos = ImGui::GetCursorScreenPos();
			switches.planetScale.selected = scene.GetSimDataDrawer().IsRealScaleEnabled();
			if (switches.planetScale.DrawBlank(buttonSize))
				scene.GetSimDataDrawer().SetRealScale(switches.planetScale.selected);
			ImGui::TextTooltipOnHover("If enabled shows real scale of simulated objects.\nOtherwise objects are always large enough to be visible on screen.");
			ImGui::SameLine();
			draw->AddCircle(cursorPos + ImVec2(buttonSize.x*0.4f, buttonSize.y*0.5f), buttonSize.x*0.3f, textCol, 12, 2.0f);
			draw->AddCircleFilled(cursorPos + buttonSize*0.75f, buttonSize.x*0.4f - style.FramePadding.x, textCol, 12);
			//LineTrails button
			cursorPos = ImGui::GetCursorScreenPos();
			switches.lineTrails.selected = scene.GetLineTrails().IsAnyEnabled();
			if (switches.lineTrails.DrawBlank(buttonSize))
				scene.SwitchLineTrails(switches.lineTrails.selected);
			ImGui::TextTooltipOnHover("Enables/Disables rendering of trails behind simulated objects.");
			ImGui::SameLine();
			draw->PathArcTo(cursorPos + style.FramePadding, buttonSize.x*0.5f, 0, 3.14f / 2);
			draw->PathStroke(textCol, false, 2.0f);
			draw->AddCircleFilled(cursorPos + buttonSize*0.6f, buttonSize.x*0.15f, textCol);
			//Grid button
			cursorPos = ImGui::GetCursorScreenPos();
			switches.grid.selected = scene.IsGridEnabled();
			if (switches.grid.DrawBlank(buttonSize))
				scene.SwitchGrid(switches.grid.selected);
			ImGui::TextTooltipOnHover("Enables/Disabled rendering of the grid.");
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
			auto greyCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
			ImVec2 cursorPos;
			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##<<Raw", buttonSize))
				viewer.SetRawMultiplier(std::max(int(viewer.GetRawMultiplier() - 10), 1));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Decreses raw multiplier by 10.\nControls speed of the simulation at expense of CPU power.\nCurrent value: %u", viewer.GetRawMultiplier());
				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			auto col = viewer.GetRawMultiplier() > 1 ? textCol : greyCol;
			DrawSlowerTriangles(draw, cursorPos, buttonSize, style, col);
			draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), cursorPos + ImVec2(buttonSize.x*0.08f, buttonSize.y*0.55f), col, "R");

			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##<<DT", buttonSize))
				viewer.SetDTMultiplier(std::max(viewer.GetDTMultiplier() / 10, size_t(1)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Divides DT multiplier by 10.\nControls speed of the simulation at expense of precision.\nCurrent value: %u", viewer.GetDTMultiplier());

				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			DrawSlowerTriangles(draw, cursorPos, buttonSize, style, viewer.GetDTMultiplier() > 1 ? textCol : greyCol);

			//Pause Button
			cursorPos = ImGui::GetCursorScreenPos();
			if (switches.pause.DrawBlank(buttonSize) && viewer.IsRunning())
			{
				viewer.PauseSimulation();
				switches.play.selected = false;
			}
			ImGui::SameLine();
			//Draw pause paralel lines
			draw->AddLine(ImVec2(cursorPos.x + buttonSize.x*0.35f, cursorPos.y + style.FramePadding.y),
						  ImVec2(cursorPos.x + buttonSize.x*0.35f, cursorPos.y + buttonSize.y - style.FramePadding.x),
						  textCol, 3.0f);
			draw->AddLine(ImVec2(cursorPos.x + buttonSize.x*0.65f, cursorPos.y + style.FramePadding.y),
						  ImVec2(cursorPos.x + buttonSize.x*0.65f, cursorPos.y + buttonSize.y - style.FramePadding.x),
						  textCol, 3.0f);
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
				viewer.SetDTMultiplier(viewer.GetDTMultiplier() * 10);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Multiplies DT multiplier by 10.\nControls speed of the simulation at expense of precision.\nCurrent value: %u", viewer.GetDTMultiplier());
				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			DrawFasterTriangles(draw, cursorPos, style, buttonSize, textCol);

			cursorPos = ImGui::GetCursorScreenPos();
			if (ImGui::Button("##>>Raw", buttonSize))
				viewer.SetRawMultiplier(viewer.GetRawMultiplier() + (viewer.GetRawMultiplier() == 1 ? 9 : 10));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Increases raw multiplier by 10.\nControls speed of the simulation at expense of CPU power.\nCurrent value: %u", viewer.GetRawMultiplier());
				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			DrawFasterTriangles(draw, cursorPos, style, buttonSize, textCol);
			draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(), cursorPos + ImVec2(buttonSize.x*0.68f, buttonSize.y*0.55f), textCol, "R");
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
		void GUIDrawer::BotttomMenuBar(SimData & data, Viewer & viewer, SceneDrawer & scene, size_t w, size_t h)
		{
			auto context = ImGui::GetCurrentContext();
			//Calculated same way as in Imgui's window calculations - function MenuBarHeight() in imgui_internal.h
			auto menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			ImGui::SetNextWindowPos(ImVec2(0, float(h) - menuBarHeight));
			ImGui::SetNextWindowContentSize(ImVec2(float(w), 0));
			if (ImGui::Begin("BottomMenuBar", nullptr, ImVec2(float(w), 0), 0.0f,
							 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::AlignFirstTextHeightToWidgets();
					auto simTime = SplitTime(viewer.GetSimTime());
					ImGui::Text("Simulated time: %u years, %u days, %u hours, %u minutes, %u seconds", simTime.Y, simTime.D, simTime.H, simTime.M, simTime.S);
					ImGui::SameLine(w / 2.0f - offsets.speedAndFollow / 2.0f);
					ImGui::BeginGroup();
					ImGui::Text("\tSpeed: %ux", viewer.GetDTMultiplier()*viewer.GetRawMultiplier());
					auto index = scene.GetActiveCam().GetFollowedObjectIndex();
					if (index != Camera::noTarget)
					{
						ImGui::SameLine();
						ImGui::Text("\tFollowing: %s", data[index].name.c_str());
						ImGui::SameLine();
						if (ImGui::SmallButton("X"))
							scene.GetActiveCam().FollowObject(Camera::noTarget);
					}
					ImGui::EndGroup();
					offsets.speedAndFollow = ImGui::GetItemRectSize().x;
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						auto speedRatio = SplitTime(uint64_t(viewer.GetDTMultiplier()*viewer.GetRawMultiplier()));
						ImGui::Text("1 real second = %u years, %u days, %u hours, %u minutes, %u seconds", speedRatio.Y, speedRatio.D, speedRatio.H, speedRatio.M, speedRatio.S);
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
					GridSize(w, context, menuBarHeight, scene);
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
		}
		void GUIDrawer::GridSize(const size_t &w, ImGuiContext * context, float menuBarHeight, solar::drawers::SceneDrawer & scene)
		{
			auto gridPos = w - offsets.grid - 2 * context->Style.FramePadding.x;
			auto draw = ImGui::GetWindowDrawList();
			ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4 {0.06f, 0.24f, 0.71f,1.0f});
			draw->AddLine(ImVec2(gridPos - 100.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 5.0f),
						  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 5.0f),
						  col, 2.5f);

			draw->AddLine(ImVec2(gridPos - 100.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 10.0f),
						  ImVec2(gridPos - 100.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
						  col, 2.5f);
			draw->AddLine(ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f - 10.0f),
						  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
						  col, 2.5f);

			draw->AddLine(ImVec2(gridPos - 50.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 5.0f),
						  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 5.0f),
						  col, 2.5f);
			draw->AddLine(ImVec2(gridPos - 50.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
						  ImVec2(gridPos - 50.0f, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 10.0f),
						  col, 2.5f);
			draw->AddLine(ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f),
						  ImVec2(gridPos, ImGui::GetCursorScreenPos().y + menuBarHeight / 2.0f + 10.0f),
						  col, 2.5f);

			double scale = scene.GetGridScale();
			std::string units = "meters";
			double gridSize = scale;

			//Change to better units if appropiate
			if ((gridSize = scale / PhysUnits::lightYear) >= 1.0)
				units = "light years";
			else if ((gridSize = scale / PhysUnits::AU) >= 1.0)
				units = "AU";
			else if ((gridSize = scale / PhysUnits::kilometer) >= 1.0)
				units = "kilometres";

			ImGui::SameLine(gridPos); ImGui::Text("%.5g / %.5g %s", gridSize, gridSize*scene.GetGrid().SmallToBig(), units.c_str());
			char buffer[255];
			sprintf_s(buffer, "%f / %f %s", gridSize, gridSize*scene.GetGrid().SmallToBig(), units.c_str());
			ImGui::TextTooltipOnHover(buffer);
			offsets.grid = ImGui::GetItemRectSize().x;
		}
	}
}
