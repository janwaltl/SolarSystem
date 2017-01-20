#include "GUIDrawer.h"

#include "Source/Viewers/IMGuiViewer.h"

#include "GUI/Visuals.h"
#include "GUI/SimProperties.h"
#include "GUI/UnitsProperties.h"
#include "GUI/MouseControls.h"
namespace solar
{
	namespace drawers
	{

		void GUIDrawer::Draw(simData_t& data, IMGuiViewer& viewer)
		{
			gui::GrabControl(viewer);
			gui::ZoomControl(viewer, viewer.GetFrameTime());
			unitsViewer(data, &viewer);
			gui::SimProperties(viewer);
			gui::Visuals(*viewer.GetTrailsDrawer(), data);
		}
	}
}
