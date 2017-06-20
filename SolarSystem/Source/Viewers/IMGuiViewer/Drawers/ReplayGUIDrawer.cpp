#include "ReplayGUIDrawer.h"
#include "Source/Viewers/ReplayerViewer.h"
#include "GUI/Visuals.h"
#include "GUI/MouseControls.h"

namespace solar
{
	namespace drawers
	{
		ReplayGUIDrawer::ReplayGUIDrawer(const std::string& replayFileName) :
			unitsProps(Vec2d(10, 10)), replayControls(replayFileName)
		{
		}

		void ReplayGUIDrawer::Draw(simData_t & data, ReplayerViewer & viewer, drawers::LineTrailsDrawer & lineTrails,
								   size_t w, size_t h)
		{
			gui::GrabControl(viewer);
			gui::ZoomControl(viewer);
			unitsProps(data, &viewer);
			replayControls(viewer, &lineTrails, w, h);
			gui::Visuals(lineTrails, data,w,h);
		}
	}
}
