#include "SimProperties.h"

#include "Source/Common/SystemUnit.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"

namespace solar
{
	namespace gui
	{
		//Constants identifying the PopUps
		constexpr char* editRawMultPopUp = "RawMultPopUp";
		constexpr char* editDTMultPopUp = "DTMultPopUp";


		//PopUp allowing to set a new RawMult
		//Helper function, not declared in header file
		void EditRawMultPopUp(SystemUnit& sys)
		{
			// Static is safe here, because there can only be one PopUp open at the same time.
			//	- Because ImGui identifies PopUp by strings, so this function always opens the same and only popUp.
			//  - And there cannot be two popUps open, because they dissappear when you click outside=on the other one.
			// So this is quick & easy way to add some needed state to it without diving into classes...
			static bool firstOpen = true;
			static int tempRawSpeed = 1;//Needs to be int because of ImGUi::InputInt

			if (ImGui::BeginPopup(editRawMultPopUp))
			{
				if (firstOpen)//On opening, set temp to current multiplier
				{
					tempRawSpeed = sys.GetRawMultiplier();
					firstOpen = false;
				}
				ImGui::InputInt("", &tempRawSpeed, 1, 100);
				if (tempRawSpeed < 1)
					tempRawSpeed = 1;

				if (static_cast<size_t>(tempRawSpeed) > sys.GetRawMultiplier())
					ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "High values may result in higher frameTimes "
									   "and higher CPU costs which in turn might result in loss of responsivness(FPS&UPS).");

				if (ImGui::SmallButton("Set"))
				{
					sys.SetRawMultiplier(static_cast<size_t>(tempRawSpeed));
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			else//If PopUp is closed
				firstOpen = true;
		}

		//PopUp allowing to set a new DTMult
		//Helper function, not declared in header file
		void EditDTMultPopUp(SystemUnit& sys)
		{
			// Static is safe here, because there can only be one PopUp open at the same time.
			//	- Because ImGui identifies PopUp by strings, so this function always opens the same and only popUp.
			//  - And there cannot be two popUps open, because they dissappear when you click outside=on the other one.
			// So this is quick & easy way to add some needed state to it without diving into classes...
			static bool firstOpen = true;
			static int tempDTSpeed = 1;//Needs to be int because of ImGUi::InputInt

			if (ImGui::BeginPopup(editDTMultPopUp))
			{
				if (firstOpen)
				{
					firstOpen = false;
					tempDTSpeed = sys.GetDTMultiplier();
				}
				ImGui::InputInt("", &tempDTSpeed, 100, 10'000);
				if (tempDTSpeed < 1)
					tempDTSpeed = 1;

				if (static_cast<size_t>(tempDTSpeed) > sys.GetDTMultiplier())
					ImGui::TextColored({1.0f,0.0f,0.0f,1.0f}, "High values may result in lost of precision "
									   "and stability of simulated system.");
				if (ImGui::SmallButton("Set"))
				{
					sys.SetDTMultiplier(static_cast<size_t>(tempDTSpeed));
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			else
				firstOpen = true;
		}

		void SimProperties(SystemUnit& sys)
		{
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiSetCond_Once);
			if (ImGui::Begin("Simulation's controls", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				SimControls(sys);
				ImGui::Separator();
				SimMetrics(sys);
				ImGui::End();
			}
		}

		void SimControls(SystemUnit& sys)
		{
			//Pause/Resume buttons
			ImGui::Text("Simulation is "); ImGui::SameLine();
			if (sys.IsRunning())
			{
				if (ImGui::StateSmallButtonWithTooltip("Running", "Pause", false))
					sys.PauseSimulation();
			}
			else
				if (ImGui::StateSmallButtonWithTooltip("Paused", "Resume", true))
					sys.ResumeSimulation();

			if (ImGui::SmallButton("Change raw speed"))
				ImGui::OpenPopup(editRawMultPopUp);

			ImGui::TextTooltipOnHover("Controls speed via rawMultiplier - at expense of CPU time");

			EditRawMultPopUp(sys);//PopUp, because of ImGui immediate architecture must be called even if not opened

			if (ImGui::SmallButton("Change DT speed"))
				ImGui::OpenPopup(editDTMultPopUp);

			ImGui::TextTooltipOnHover("Controls speed via DTMultiplier");

			EditDTMultPopUp(sys);//PopUp, because of ImGui immediate architecture must be called even if not opened

			if (sys.IsPaused())//Stepping simulation
			{
				if (ImGui::SmallButton("Make Step##Step"))
					sys.StepSimulation();
				ImGui::TextTooltipOnHover("Makes one step forwards");
			}
			else
				ImGui::NewLine();//Blank line for formatting purposes.
		}

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

			ImGui::Text("SimTime"); ImGui::NextColumn();
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
