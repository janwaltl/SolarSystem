#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "Source/Common/Common.h"
#include "GUI/UnitsProperties.h"

namespace solar
{
	class IMGuiViewer;

	namespace drawers
	{
		//Renders User interface via ImGUI
		class GUIDrawer
		{
		public:
			void Draw(simData_t& data, IMGuiViewer& viewer);
		private:
			gui::UnitsViewer unitsViewer;

			simData_t* simData;
			IMGuiViewer* viewer;
		};
	}
}


#endif