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
		GUIDrawer::GUIDrawer(IMGuiViewer * parent, simData_t* data) :
			viewer(parent), follow(false), simData(data)
		{
			assert(viewer);
			assert(simData);
		}
		void GUIDrawer::Draw()
		{
			gui::GrabControl(*viewer);
			gui::ZoomControl(*viewer, viewer->GetFrameTime());

			unitsViewer(*simData, viewer);
			gui::SimProperties(*viewer);
			gui::Visuals(*viewer->GetTrailsDrawer(), *simData);
		}
	}
}
