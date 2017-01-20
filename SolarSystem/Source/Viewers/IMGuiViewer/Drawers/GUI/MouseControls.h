#ifndef IMGUIVIEWER_GUI_MOUSE_CONTROLS_HEADER
#define IMGUIVIEWER_GUI_MOUSE_CONTROLS_HEADER

#include "Source/Common/Common.h"

namespace solar
{
	class OMSAR;
	namespace gui
	{
		//Zoom & Grab controls
		void MouseControl(OMSAR& sys, double frameTime);
		//Zooms the screen base on mouse's wheel scrolling
		void ZoomControl(OMSAR& sys, double frameTime);
		//Drags the screen via mouse
		void GrabControl(OMSAR& sys);
	}
}

#endif