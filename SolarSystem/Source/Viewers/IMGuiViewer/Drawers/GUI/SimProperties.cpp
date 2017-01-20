#include "SimProperties.h"

#include "Source/Common/SystemUnit.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"

namespace solar
{
	namespace gui
	{
		void SimMetrics(const SystemUnit & sys)
		{
			auto numCols = ImGui::GetColumnsCount();
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 120.0f);

			ImGui::Text("RawMultiplier"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("For each Dtime passed time, simMethod will be called this many times.\n"
									  "Controls speed of the simulation at expense of CPU power.");
			ImGui::Text("%10i ", sys.GetRawMultiplier()); ImGui::NextColumn();

			ImGui::Text("DTMultiplier"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("dTime*this will be passed to simMethod as dTime.\n"
									  "Controls speed of the simulation at expense of precision.");
			ImGui::Text("%10i ", sys.GetDTMultiplier()); ImGui::NextColumn();


			ImGui::Text("Speed ratio"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("This amount of time is simulated each passed second.");
			size_t speedRatio = sys.GetRawMultiplier()*sys.GetDTMultiplier();
			auto spDays = (speedRatio % 31'536'000) / 86'400;
			auto spHours = (speedRatio % 86'400) / 3600;
			auto spMins = (speedRatio % 3600) / 60;
			auto spSecs = speedRatio % 60;
			ImGui::Text("%id %ih %im %is", spDays, spHours, spMins, spSecs); ImGui::NextColumn();
			ImGui::Text("Stable"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("Whether the simulation can run at set speed at real-time."
									  " \nMeans that it is not lagging behind = frameTime<dTime ");
			if (sys.GetDtime() > sys.GetFrameTime())
				ImGui::TextColored({0.0f,1.0f,0.0f,1.0f}, "YES");
			else
				ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "NO");
			ImGui::NextColumn();

			ImGui::Text("Delta time"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("Basic unit of simulated time.");
			ImGui::Text("%07.4f ms", sys.GetDtime()*1000.0); ImGui::NextColumn();

			ImGui::Text("Frame time"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("Amount of time last frame(sim + viewer) took.");
			ImGui::Text("%07.4f ms", sys.GetFrameTime()*1000.0); ImGui::NextColumn();

			ImGui::Text("Real-time"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("How long has been the simulation running.");
			auto runTime = sys.GetRunTime();
			auto mins = floor(runTime / 60);
			runTime -= mins * 60;
			ImGui::Text("%3.0fm %02.1fs", mins, runTime); ImGui::NextColumn();

			ImGui::Text("Sim-time"); ImGui::NextColumn();
			ImGui::TextTooltipOnHover("Amount of time simulated.");
			auto simTime = sys.GetSimTime();
			auto years = floor(simTime / 31'536'000);
			simTime -= years * 31'536'000;
			auto days = simTime / 86'400;
			ImGui::Text("%3.0fy %03.0fd", years, days); ImGui::NextColumn();

			ImGui::Columns(numCols);
		}
	}
}
