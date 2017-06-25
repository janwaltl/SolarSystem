#ifndef IMGUIVIEWER_GUI_MOUSE_CONTROLS_HEADER
#define IMGUIVIEWER_GUI_MOUSE_CONTROLS_HEADER

namespace solar
{
	class OMSAR;
	class Camera;
	namespace gui
	{
		//Zoom & Grab controls
		//void MouseControl(OMSAR& sys);
		//Zooms the screen base on mouse's wheel scrolling
		void ZoomControl(OMSAR& sys);
		//Drags the screen via mouse
		void GrabControl(Camera& cam);
	}
}

#endif