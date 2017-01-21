#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "Source/Common/Common.h"
#include "GUI/UnitsProperties.h"
#include "LineTrailsDrawer.h"
namespace solar
{
	class IMGuiViewer;
	namespace drawers
	{
		//Renders User interface for simulation via ImGUI
		class GUIDrawer
		{
		public:
			void Draw(simData_t& data, IMGuiViewer& viewer, drawers::LineTrailsDrawer& lineTrails);
		private:
			gui::UnitsProperties unitsProps;
		};
	}
}


#endif