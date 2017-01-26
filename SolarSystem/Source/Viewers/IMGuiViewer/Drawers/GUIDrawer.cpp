#include "GUIDrawer.h"

#include "Source/Viewers/IMGuiViewer.h"

#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
#include "GUI/MouseControls.h"

namespace solar
{
	namespace drawers
	{
		void GUIDrawer::Draw(simData_t& data, IMGuiViewer& viewer, drawers::LineTrailsDrawer& lineTrails)
		{
			gui::GrabControl(viewer);
			gui::ZoomControl(viewer);
			unitsProps(data, &viewer);
			gui::SimProperties(viewer);
			gui::Visuals(lineTrails, data);
		}
	}
}
