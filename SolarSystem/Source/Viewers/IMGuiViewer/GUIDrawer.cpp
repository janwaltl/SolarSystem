#include "GUIDrawer.h"

#include "IMGuiLibrary/imgui.h"
#include "../IMGuiViewer.h"

namespace solar
{
	GUIDrawer::GUIDrawer(IMGuiViewer * parent) :
		viewer(parent), follow(false)
	{
		assert(viewer);
	}
	void GUIDrawer::Draw(simData_t& data)
	{
		this->data = &data;
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(150, 400), ImGuiSetCond_Once);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f,0.0f,0.0f,0.0f});
		ImGui::Begin("Window",NULL,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoCollapse);
		ImGui::Text("%.6f",length(data[3].pos));
		ImGui::End();
		ImGui::PopStyleColor();

		ControlsWin();

	}
	void GUIDrawer::ControlsWin()
	{
		ImGui::SetNextWindowPos(ImVec2(1000, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiSetCond_Once);

		if (ImGui::Begin("Controls"))
		{
			ZoomControl();
			if (!follow)
			{
				ManualControls();
				OfferFollow();
			}
			else//following a planet
				Following();
			ImGui::End();
		}
	}
	void GUIDrawer::ManualControls()
	{
		auto offset = viewer->GetOffset();
		float x = static_cast<float>(offset.X());
		ImGui::SliderFloat("X##position", &x, -1.0f, 1.0f);//Normalized coordinates
		float y = static_cast<float>(offset.Y());
		ImGui::SliderFloat("Y##position", &y, -1.0f, 1.0f);//Normalized coordinates
		viewer->Move({x,y});
	}
	void GUIDrawer::ZoomControl()
	{
		//Clamp values were chosen to work well with AU units,
		//10.0f means, that slider acts like f(x)=x^10, thus being more smooth around zero, which allows for
		//"slower" and more precise zoom on planet-scale.
		float tmp = static_cast<float>(viewer->ScaleFactor());
		ImGui::SliderFloat("Zoom", &tmp, 0.0001f, 100.0f, "%.4f", 10.0f);
		viewer->ScaleFactor(tmp);
	}
	void GUIDrawer::Following()
	{
		viewer->Move(-1.0*viewer->ScaleFactor()*(*data)[8].pos);
		if (ImGui::Button("Cancel"))//Revert to manual controls
		{
			follow = false;
			viewer->Move({});//Centers around the sun
			viewer->ResetZoom(*data);//Zooms to see whole solar system
		}
	}
	void GUIDrawer::OfferFollow()
	{
		ImGui::Checkbox("Follow Mars", &follow);
	}
}
