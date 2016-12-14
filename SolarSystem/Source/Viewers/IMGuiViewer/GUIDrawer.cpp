#include "GUIDrawer.h"

#include "IMGuiLibrary/imgui.h"

namespace solar
{

	void GUIDrawer::Draw(simData_t& data)
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(150, 400), ImGuiSetCond_Once);
		ImGui::Begin("Window");
		ImGui::Text(std::to_string(length(data[3].pos)).c_str());
		ImGui::Button("Button");
		ImGui::End();
		ImGui::Render();
	}
}
