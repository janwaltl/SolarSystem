#include "ReplayControls.h"

#include <fstream>
#include <algorithm>
#include "Source/Units/SystemUnit.h"
#include "Source/Common/Exception.h"

#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imgui.h"
#include "Source/Viewers/IMGuiViewer/Drawers/LineTrailsDrawer.h"

namespace solar
{
	namespace gui
	{
		ReplayControls::ReplayControls(const std::string & replayFileName) :
			fileName(replayFileName), recordNum(1), speed(1.0), tmpRecordNum(1), jumped(false)
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

		void ReplayControls::operator()(SystemUnit & sys, drawers::LineTrailsDrawer* lineTrails, size_t w, size_t h)
		{
			recordNum = GetRecordNum(sys.GetSimTime());

			if (recordNum >= numRecords)
			{
				sys.PauseSimulation();
				recordNum = numRecords;
			}
			ImGui::SetNextWindowPos(ImVec2(10.0f, float(h-80.0f)), ImGuiSetCond_Once);
			ImGui::SetNextWindowSize(ImVec2(float(w-10.0f), 80.0f), ImGuiSetCond_Once);
			if (ImGui::Begin("Replay controls", NULL, ImGuiWindowFlags_NoCollapse |
							 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				ImGui::Columns(2, "##ReplayInfo", false);
				ImGui::SetColumnOffset(1, 520.0f);//To fit ControlButtons
				ControlButtons(sys, lineTrails);
				ImGui::NextColumn();
				ReplayInfo(sys);
				ImGui::Columns(1);
				ProgressBar();
			}
			ImGui::End();

		}
		void ReplayControls::ControlButtons(SystemUnit & sys, drawers::LineTrailsDrawer* lineTrails)
		{
			//BUG FIX:
			//After changing simTime, old data can still get pushed into lineTrails
			// because new data will be fetched only after call to simMethod, which can be as much as 'deltaT' time
			// delayed, but in between, there can be unlimited number of calls to viewer and thus to lineTrails.
			// This ensures that they are cleared until new data arrive. Because change in simTime implies call to simMethod
			// and consequently new data.
			if (jumped && jumpTime == sys.GetSimTime())
			{
				lineTrails->ClearAll();
			}
			else if (jumped)
			{
				lineTrails->ClearAll();
				jumped = false;
			}

			if (ImGui::Button("Slow down"))
			{
				speed /= 2.0;
				sys.SetDTMultiplier(static_cast<uint32_t>(multiplier*speed));
			}
			ImGui::SameLine();

			if (recordNum < numRecords)//Not the last record
			{
				if (ImGui::Button(sys.IsPaused() ? "Resume" : "Pause"))
					sys.IsPaused() ? sys.ResumeSimulation() : sys.PauseSimulation();
			}
			else
				ImGui::Button("Finished");

			ImGui::SameLine();

			if (ImGui::Button("Speed up"))
			{
				speed *= 2.0;
				sys.SetDTMultiplier(static_cast<uint32_t>(multiplier*speed));
			}
			ImGui::SameLine();

			ImGui::Text("Jump To Record:"); ImGui::SameLine();
			ImGui::PushItemWidth(100);
			ImGui::InputInt("", &tmpRecordNum); ImGui::SameLine();

			tmpRecordNum = std::max(1, std::min(tmpRecordNum, (int)numRecords));//Clamp the value

			if (ImGui::Button("Jump"))
			{
				SetSimTimeBasedOnRecordNum(sys, tmpRecordNum);
				jumped = true;
				jumpTime = sys.GetSimTime();
				lineTrails->ClearAll();
			}

			ImGui::PopItemWidth();
		}

		void ReplayControls::ReplayInfo(SystemUnit & sys)
		{
			const static ImVec4 whiteCol {1.0,1.0,1.0,1.0};

			ImGui::Text("Replay file:"); ImGui::SameLine();
			ImGui::TextColored(whiteCol, fileName.c_str()); ImGui::SameLine();
			ImGui::Text("SimTime:"); ImGui::SameLine();
			ImGui::TextTooltipOnHover("Amount of simulated time.");
			auto time = SplitTime(sys.GetSimTime());
			ImGui::TextColored(whiteCol, "%3iy %3id %3ih", time.Y, time.D, time.H);
			ImGui::SameLine();
			ImGui::Text("Speed:"); ImGui::SameLine();
			if (speed >= 0.99f)//>=1.0
				ImGui::TextColored(whiteCol, "%.0fx", speed);
			else
				ImGui::TextColored(whiteCol, "1/%.0fx", 1.0 / speed);
		}

		void ReplayControls::ProgressBar()
		{
			std::string barText = std::string("Record ") + std::to_string(recordNum) +
				'/' + std::to_string(numRecords);
			ImGui::ProgressBar(recordNum / float(numRecords), ImVec2(-1, 20), barText.c_str());
		}

		uint32_t ReplayControls::GetRecordNum(simulatedTime simTime)
		{
			return static_cast<uint32_t>(simTime / multiplier / dTime) + 1;
		}
		void ReplayControls::SetSimTimeBasedOnRecordNum(SystemUnit & sys, uint32_t newRecordNum)
		{
			//32bits can overflow when counting in seconds
			sys.SetSimTime(simulatedTime(uint64_t(newRecordNum - 1)*multiplier*dTime));
		}
	}
}
