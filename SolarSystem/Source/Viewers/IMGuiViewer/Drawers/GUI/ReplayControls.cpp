#include "ReplayControls.h"

#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Common/SystemUnit.h"
#include <fstream>
#include "Source/Exception.h"
#include <algorithm>

namespace solar
{
	namespace gui
	{
		ReplayControls::ReplayControls(const std::string & replayFileName) :
			fileName(replayFileName), recordNum(1), speed(1.0), tmpRecordNum(1)
		{
			std::ifstream in(fileName, std::ios::binary);
			if (!in.is_open())
				throw Exception("Cannot open replay file '" + replayFileName + "'.");

			char magic[2];
			in.read(magic, sizeof(magic));
			if (magic[0] != 'R' || magic[1] != 'E')
				throw Exception("File '" + replayFileName + "' does not contain a replay");

			in.read(reinterpret_cast<char*>(&dTime), sizeof(dTime));
			in.read(reinterpret_cast<char*>(&multiplier), sizeof(multiplier));
			in.read(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));
			in.close();
		}
		void ReplayControls::operator()(SystemUnit & sys)
		{
			recordNum = GetRecordNum(sys.GetSimTime());

			ImGui::SetNextWindowPos(ImVec2(10, 620), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(1000, 80), ImGuiSetCond_Once);
			if (ImGui::Begin("Replay controls", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				ImGui::Columns(2, "##ReplayInfo", false);
				ImGui::SetColumnOffset(1, 500.0f);
				ControlButtons(sys);
				ImGui::NextColumn();
				ReplayInfo(sys);
				ImGui::Columns(1);
				ProgressBar();
				ImGui::End();
			}
		}
		void ReplayControls::ControlButtons(SystemUnit & sys)
		{
			if (ImGui::Button("Slow down"))
			{
				speed /= 2.0;
				sys.SetDTMultiplier(multiplier*speed);
			}
			ImGui::SameLine();

			if (ImGui::Button(sys.IsPaused() ? "Resume" : "Pause"))
				sys.IsPaused() ? sys.ResumeSimulation() : sys.PauseSimulation();
			ImGui::SameLine();

			if(ImGui::Button("Speed up"))
			{
				speed *= 2.0;
				sys.SetDTMultiplier(multiplier*speed);
			}
			ImGui::SameLine();

			ImGui::Text("Jump To Record:"); ImGui::SameLine();
			ImGui::PushItemWidth(100);
			ImGui::InputInt("", &tmpRecordNum); ImGui::SameLine();
			tmpRecordNum = std::max(1, std::min(tmpRecordNum, (int)numRecords));
			if (ImGui::Button("Jump"))
				SetSimTimeBasedOnRecordNum(sys, tmpRecordNum);
			ImGui::PopItemWidth();
		}

		void ReplayControls::ReplayInfo(SystemUnit & sys)
		{
			const static ImVec4 whiteCol {1.0,1.0,1.0,1.0};

			ImGui::Text("Replay file:"); ImGui::SameLine();
			ImGui::TextColored(whiteCol, fileName.c_str()); ImGui::SameLine();
			ImGui::Text("SimTime:"); ImGui::SameLine();
			ImGui::TextTooltipOnHover("Amount of simulated time.");
			auto simTime = sys.GetSimTime();
			uint32_t years = simTime / 31'536'000;
			simTime -= years * 31'536'000;
			uint32_t days = simTime / 86'400;
			simTime -= days * 86'400;
			uint32_t hours = simTime / 3'600;
			ImGui::TextColored(whiteCol, "%3iy %3id %3ih", years, days, hours);
			ImGui::SameLine();
			ImGui::Text("Speed: %.2fx",speed);
		}

		void ReplayControls::ProgressBar()
		{
			std::string barText = std::string("Record ") + std::to_string(recordNum) +
				'/' + std::to_string(numRecords);
			ImGui::ProgressBar(recordNum / float(numRecords), ImVec2(-1, 20), barText.c_str());
		}

		uint32_t ReplayControls::GetRecordNum(double simTime)
		{
			return simTime / multiplier / dTime + 1;
		}
		void ReplayControls::SetSimTimeBasedOnRecordNum(SystemUnit & sys, uint32_t recordNum)
		{
			sys.SetSimTime((recordNum - 1)*multiplier*dTime);
		}
	}
}
