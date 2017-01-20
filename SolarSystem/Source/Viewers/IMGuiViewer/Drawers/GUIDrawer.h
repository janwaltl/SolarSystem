#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "Source/Common/Common.h"

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
			void ZoomControl();
			// Manually control position of the simulation
			void ManualControls();
			// Following a Unit (Planet)
			void Following();
			// Window with simulation's controls
			void ControlsWin();
			// Controls speed of simulation
			void SimControls();
			void SpeedControl();
			void EditRawMultPopUp();
			void EditDTMultPopUp();
			// Shows list of all units and their properties
			void UnitsViewer();

			// Translating vectors for ManualControls
			Vec2 offset, drag;
			// Whether an unit is being followed
			bool follow;
			// Speed control
			const char* editRawMultPopUp = "EditRawMult";
			const char* editDTMultPopUp = "EditDTMult";
			int tempRawSpeed, tempDTSpeed;

			//Currently selected Unit in comboBox
			int selectedUnit;//-1 if none
			int selectedRefSystem;//To which reference system are Unit's properties displayed
								  //Unit that serves as default centered reference system to which are Unit's units displayed
			Unit centerSystem;

			simData_t* simData;
			IMGuiViewer* viewer;
		};
	}
}


#endif