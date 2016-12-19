#ifndef IMGUIVIEWER_GUIDRAWER_HEADER
#define IMGUIVIEWER_GUIDRAWER_HEADER


#include "../../Common/Common.h"

namespace solar
{
	class IMGuiViewer;
	//Renders User interface via ImGUI
	class GUIDrawer
	{
	public:
		GUIDrawer(IMGuiViewer* parent);
		void Draw(simData_t& data);
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
			// Shows dTime, frameTime, simTime and other statistics of the simulation
			void SimMetrics();
			// Shows list of all units
			void UnitsViewer();
		// Translating vectors for ManualControls
		Vec2 offset, drag;
		// Whether an unit is being followed
		bool follow;
		// Speed control
		const char* editRawMultPopUp = "EditRawMult";
		const char* editDTMultPopUp="EditDTMult";
		int tempRawSpeed, tempDTSpeed;

		IMGuiViewer* viewer;
		simData_t* simData;
	};
}


#endif