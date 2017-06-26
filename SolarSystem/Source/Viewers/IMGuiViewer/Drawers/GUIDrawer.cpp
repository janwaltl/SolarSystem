#include "GUIDrawer.h"


#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
#include "GUI/MouseControls.h"
#include "Source/Viewers/IMGuiViewer.h"

namespace solar
{
	namespace drawers
	{
		void GUIDrawer::Draw(simData_t& data, IMGuiViewer& viewer, drawers::LineTrailsDrawer& lineTrails,
							 size_t w, size_t h)
		{
			mouseControls(viewer.GetCamera());
			unitsProps(data);
			gui::SimProperties(viewer);
			gui::Visuals(lineTrails, data, w, h);
		}
	}
}
