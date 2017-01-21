#include "ReplayGUIDrawer.h"
#include "GUI/Visuals.h"
#include "GUI/MouseControls.h"
#include "Source/Viewers/ReplayerViewer.h"

namespace solar
{
	namespace drawers
	{
		ReplayGUIDrawer::ReplayGUIDrawer() :
			unitsProps(Vec2(10, 10))
		{
		}

		void ReplayGUIDrawer::Draw(simData_t & data, ReplayerViewer & viewer, drawers::LineTrailsDrawer & lineTrails)
		{
			gui::GrabControl(viewer);
			gui::ZoomControl(viewer, viewer.GetFrameTime());
			unitsProps(data, &viewer);
			gui::Visuals(lineTrails, data);
		}
	}
}
