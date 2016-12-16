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
		void ControlsWin();
		//Manually control position of the simulation
		void ManualControls();
		void ZoomControl();
		//Following a Unit -> automatically set pos
		void Following();
		void OfferFollow();
		bool follow;
		IMGuiViewer* viewer;
		simData_t* data;
	};
}


#endif