#ifndef IMGUIVIEWER_GUI_REPLAY_CONTROLS_HEADER
#define IMGUIVIEWER_GUI_REPLAY_CONTROLS_HEADER

#include "Source/Common/Common.h"

namespace solar
{
	class SystemUnit;

	namespace gui
	{
		class ReplayControls
		{
		public:
			ReplayControls(const std::string& replayFileName);
			void operator()(SystemUnit& sys);
		private:
			void ControlButtons(SystemUnit& sys);
			void ReplayInfo(SystemUnit& sys);
			void ProgressBar();
			//Gets recordNumber based on simulation time
			uint32_t GetRecordNum(double simTime);
			void SetSimTimeBasedOnRecordNum(SystemUnit& sys, uint32_t recordNum);
			
			double dTime;
			uint32_t numRecords;
			uint32_t multiplier;
			uint32_t recordNum;
			int tmpRecordNum;
			double speed;
			std::string fileName;

		};
	}
}


#endif