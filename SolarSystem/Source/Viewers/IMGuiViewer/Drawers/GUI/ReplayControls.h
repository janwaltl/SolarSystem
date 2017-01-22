#ifndef IMGUIVIEWER_GUI_REPLAY_CONTROLS_HEADER
#define IMGUIVIEWER_GUI_REPLAY_CONTROLS_HEADER

#include "Source/Common/Common.h"

namespace solar
{
	class SystemUnit;
	namespace drawers
	{
		class LineTrailsDrawer;
	}
	namespace gui
	{
		//Renders window with that contains controls for replaying a simulation
		class ReplayControls
		{
		public:
			ReplayControls(const std::string& replayFileName);
			void operator()(SystemUnit& sys, drawers::LineTrailsDrawer& lineTrails);
		private:
			void ControlButtons(SystemUnit& sys, drawers::LineTrailsDrawer& lineTrails);
			void ReplayInfo(SystemUnit& sys);
			void ProgressBar();
			//Gets recordNumber based on simulated time
			uint32_t GetRecordNum(double simTime);
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
			//cleans trails after jump
			bool cleanTrails;
		};
	}
}


#endif