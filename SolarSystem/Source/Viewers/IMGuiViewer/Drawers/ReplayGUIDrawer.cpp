#include "ReplayGUIDrawer.h"
#include "GUI/Visuals.h"
#include "GUI/MouseControls.h"
#include "Source/Viewers/ReplayerViewer.h"

void solar::drawers::ReplayGUIDrawer::Draw(simData_t & data, ReplayerViewer & viewer, drawers::LineTrailsDrawer & lineTrails)
{
	gui::GrabControl(viewer);
	gui::ZoomControl(viewer, viewer.GetFrameTime());
	unitsProps(data, &viewer);
	gui::Visuals(lineTrails, data);
}
