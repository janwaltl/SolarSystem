#ifndef DRAWERS_REPLAY_GUIDRAWER_HEADER
#define DRAWERS_REPLAY_GUIDRAWER_HEADER


#include "GUI/UnitsProperties.h"
#include "GUI/ReplayControls.h"
#include "LineTrailsDrawer.h"

namespace solar
{
	class ReplayerViewer;

	namespace drawers
	{
		//Renders UserInface for replayed simulation
		class ReplayGUIDrawer
		{
		public:
			ReplayGUIDrawer(const std::string& replayFileName);
			void Draw(simData_t& data, ReplayerViewer& viewer, drawers::LineTrailsDrawer& lineTrails);
		private:
			gui::UnitsProperties unitsProps;
			gui::ReplayControls replayControls;
		};
	}
}

#endif