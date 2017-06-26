#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "GUI/UnitsProperties.h"
#include "GUI/MouseControls.h"
#include "LineTrailsDrawer.h"
#include "Source/Units/Unit.h"

namespace solar
{
	class IMGuiViewer;
	namespace drawers
	{
		//Renders User interface for simulation via ImGUI
		class GUIDrawer
		{
		public:
			void Draw(simData_t& data, IMGuiViewer& viewer, drawers::LineTrailsDrawer& lineTrails,
					  size_t w, size_t h);
		private:
			gui::UnitsProperties unitsProps;
			gui::MouseControls mouseControls;
		};
	}
}


#endif