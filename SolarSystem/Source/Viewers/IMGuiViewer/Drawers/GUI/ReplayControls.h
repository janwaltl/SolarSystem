#ifndef IMGUIVIEWER_GUI_REPLAY_CONTROLS_HEADER
#define IMGUIVIEWER_GUI_REPLAY_CONTROLS_HEADER

#include <string>
#include "Source/Common/Timing.h"

namespace solar
{
	class SystemUnit;

	namespace drawers
	{
		class LineTrailsDrawer;
	}
	namespace gui
	{
		//Renders window that contains controls for replaying a simulation
		class ReplayControls
		{
		public:
			ReplayControls(const std::string& replayFileName);
			//Draws the window
			//SystemUnit is used to get info about simulation
			//lineTrails are cleaned after jumping
			void operator()(SystemUnit& sys, drawers::LineTrailsDrawer* lineTrails, size_t w, size_t h);
		private:
			void ControlButtons(SystemUnit& sys, drawers::LineTrailsDrawer* lineTrails);
			void ReplayInfo(SystemUnit& sys);
			void ProgressBar();
			//Gets recordNumber based on simulated time
			uint32_t GetRecordNum(simulatedTime simTime);
			void SetSimTimeBasedOnRecordNum(SystemUnit& sys, uint32_t recordNum);

			double dTime;
			uint32_t numRecords;
			uint32_t multiplier;
			uint32_t recordNum;
			//For ImGui::InputInt
			int tmpRecordNum;
			//Controls speed of replaying, multiples of multiplier
			double speed;
			//Replay's filename
			std::string fileName;
			simulatedTime jumpTime;
			bool jumped;
		};
	}
}


#endif