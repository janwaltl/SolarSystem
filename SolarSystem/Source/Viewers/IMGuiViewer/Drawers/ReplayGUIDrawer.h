#ifndef DRAWERS_REPLAY_GUIDRAWER_HEADER
#define DRAWERS_REPLAY_GUIDRAWER_HEADER

#include "Source/Common/Common.h"
#include "GUI/UnitsProperties.h"
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
			ReplayGUIDrawer();
			void Draw(simData_t& data, ReplayerViewer& viewer, drawers::LineTrailsDrawer& lineTrails);
		private:
			gui::UnitsProperties unitsProps;
		};
	}
}

#endif