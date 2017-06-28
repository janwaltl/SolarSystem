#ifndef DRAWERS_REPLAY_GUIDRAWER_HEADER
#define DRAWERS_REPLAY_GUIDRAWER_HEADER


#include "GUI/UnitsProperties.h"
#include "GUI/ReplayControls.h"
#include "GUI/CameraControls.h"
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
			void Draw(SimData& data, ReplayerViewer& viewer, drawers::LineTrailsDrawer& lineTrails,
					  size_t w, size_t h);
		private:
			gui::UnitsProperties unitsProps;
			gui::CameraControls camControls;
			gui::ReplayControls replayControls;
		};
	}
}

#endif