#include "ReplayGUIDrawer.h"
#include "Source/Viewers/ReplayerViewer.h"
#include "GUI/Visuals.h"
#include "GUI/MouseControls.h"
namespace solar
{
	namespace drawers
	{
		ReplayGUIDrawer::ReplayGUIDrawer(const std::string& replayFileName) :
			unitsProps(Vec2(10, 10)), replayControls(replayFileName)
		{
		}

		void ReplayGUIDrawer::Draw(simData_t & data, ReplayerViewer & viewer, drawers::LineTrailsDrawer & lineTrails)
		{
			gui::GrabControl(viewer);
			gui::ZoomControl(viewer, viewer.GetFrameTime());
			unitsProps(data, &viewer);
			replayControls(viewer);
			gui::Visuals(lineTrails, data);
		}
	}
}
