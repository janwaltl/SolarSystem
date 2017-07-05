#include "Graphs.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imguiExtended.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

namespace solar
{
	namespace
	{
		const char* newGraphPopUp = "NewGraph##PopUp";
		const size_t noneSelected = SIZE_MAX;
	}
	gui::Graphs::Graphs() :
		graph(ImVec2(150, 150), ImVec2(-1, 1), ImVec2(-1, 1))
	{
		showPreview = true;
		selected = noneSelected;
		graphs.emplace_back(std::make_unique<ImGuiE::LineGraph>(ImVec2(150, 150), ImVec2(-1, 1), ImVec2(-1, 1)));
		graphs.emplace_back(std::make_unique<ImGuiE::LineGraph>(ImVec2(120, 120), ImVec2(-1, 1), ImVec2(-1, 2)));
		graphs.emplace_back(std::make_unique<ImGuiE::LineGraph>(ImVec2(100, 100), ImVec2(-2, 2), ImVec2(-2, 1)));
	}
	void gui::Graphs::operator()(SimData & data)
	{
		static size_t counter = 0;
		size_t t = clock() / double(CLOCKS_PER_SEC) * 10;
		if (t != counter)
		{
			counter = t;
			graph.AddPoint(ImVec2(data[1].pos.x, data[1].pos.y));
		}
		ImGui::SetNextWindowPos(ImVec2(300, 100), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(showPreview ? 730 : 500, 250), ImGuiSetCond_Always);
		if (ImGui::Begin("Graphs", NULL))
		{
			ImGui::BeginGroup();
			ImGui::AlignFirstTextHeightToWidgets();
			ImGui::Text("List of graphs:"); ImGui::SameLine(415);
			if (ImGui::Button("New graph"))
				ImGui::OpenPopup(newGraphPopUp);
			NewGraphPopUp();

			ListOfGraphs();
			//Bottom controls
			ImGui::Button("Pause"); ImGui::SameLine();
			ImGui::Button("Remove"); ImGui::SameLine();
			ImGui::Button("Open in separate window");
			ImGui::SameLine(395);
			if (ImGui::Button(showPreview ? "Hide preview" : "Show preview"))
				showPreview = !showPreview;
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
			Preview();
			ImGui::EndGroup();
		}
		ImGui::End();
	}
	void gui::Graphs::Preview()
	{
		ImGui::NewLine();
		if (selected != noneSelected)
			graphs[selected]->Draw();
		else
			ImGui::Text("No graph is selected.");
	}
	void gui::Graphs::NewGraphPopUp()
	{
		if (ImGui::BeginPopupModal(newGraphPopUp, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}
	void gui::Graphs::ListOfGraphs()
	{
		if (ImGui::BeginChild("List of graphs", ImVec2(500, 150), false, ImGuiWindowFlags_AlwaysUseWindowPadding))
		{
			ImGui::Columns(3);
			ImGui::SetColumnOffset(1, 20);
			ImGui::SetColumnOffset(2, 400);
			size_t i = 0;
			for (const auto& g : graphs)
			{
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.90f, 0.78f, 0.00f, 1.00f));
				if (ImGui::Selectable(std::to_string(i + 1).c_str(), i == selected, ImGuiSelectableFlags_SpanAllColumns))
					selected = i;
				ImGui::PopStyleColor();
				ImGui::NextColumn();
				ImGui::Text("Distance  of Earth from the Sun");
				ImGui::NextColumn();
				ImGui::Text("Tracking");
				ImGui::NextColumn();
				ImGui::Separator();
				ImGui::PopID();
				i++;
			}
		}
		ImGui::EndChild();
	}
}