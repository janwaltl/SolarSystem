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
		// Manually control position of the simulation
		void ManualControls();
		// Zooms in and out using wheel mouse button
		void ZoomControl();
		// Following a Unit (Planet)
		void Following();
		// Offer to follow a Unit
		void OfferFollow();
		// Translating vectors
		Vec2 offset, drag;

		bool follow;
		IMGuiViewer* viewer;
		simData_t* simData;
	};
}


#endif