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
			GUIDrawer(IMGuiViewer* parent, simData_t* data);
			void Draw();
		private:
			// Control viewing of simulation - moving(following) and zooming
			void ViewControl();
			// Zooms in and out using wheel mouse button
			//void ZoomControl();
			// Manually control position of the simulation
			//void ManualControls();

			// Window with simulation's controls
			void ControlsWin();


			// Translating vectors for ManualControls
			Vec2 offset, drag;
			// Whether an unit is being followed
			bool follow;
			
			gui::UnitsViewer unitsViewer;

			simData_t* simData;
			IMGuiViewer* viewer;
		};
	}
}


#endif