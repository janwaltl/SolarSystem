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

			gui::GrabControl(viewer.GetCamera());
			gui::ZoomControl(viewer);
			unitsProps(data, &viewer);
			gui::SimProperties(viewer);
			gui::Visuals(lineTrails, data, w, h);
		}
	}
}
