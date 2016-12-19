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
		// Window with simulation's controls
		void ControlsWin();
		void SimControls();
		// Controls speed of simulation
		void SpeedControl();
		void EditRawMultPopUp();
		void EditDTMultPopUp();
		// Shows dTime, frameTime, simTime and other statistics of the simulation
		void SimMetrics();
		// Manually control position of the simulation
		void ManualControls();
		// Zooms in and out using wheel mouse button
		void ZoomControl();
		// Following a Unit (Planet)
		void Following();
		// Offer to follow a Unit
		void OfferFollow();
		// Translating vectors for manualControls
		Vec2 offset, drag;
		// Whether a unit is being followed
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