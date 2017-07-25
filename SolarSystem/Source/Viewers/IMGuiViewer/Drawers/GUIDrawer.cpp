#include "GUIDrawer.h"


#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
#include "GUI/CameraControls.h"
#include "Source/Viewers/IMGuiViewer.h"

namespace solar
{
	namespace drawers
	{
		void GUIDrawer::Draw(SimData& data, IMGuiViewer& viewer, drawers::LineTrailsDrawer& lineTrails,
							 size_t w, size_t h)
		{
			camControls(viewer.GetCamera(),data);
			//unitsProps(data);
			graphs(data, viewer.GetRunTime(), viewer.GetSimTime());
			gui::SimProperties(viewer);
			//gui::Visuals(lineTrails, data, w, h);
		}
	}
}
