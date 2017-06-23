#include "IMGuiViewer.h"

namespace solar
{
	IMGuiViewer::IMGuiViewer(size_t width, size_t height, const std::string& title) :
		OMSAR(1.0, width / double(height), Vec2d(0.0, 0.0)),
		openGL(width, height, title), imguiBackend(openGL.GetWin()),
		w(width), h(height)
	{}

	void IMGuiViewer::Prepare()
	{
		cam.Bind();
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(cam);
		GUIDrawer = std::make_unique<drawers::GUIDrawer>();
		lineTrailsDrawer = std::make_unique<drawers::LineTrailsDrawer>(data->size(), cam);

		ResetZoom(*data, 0.8);
		cam.MakeOrtho(17*2,10*2, 0.1f, 10000.0f);
		//cam.MakePerspective(90, 1.7f, 0.1f, 10000.0f);
		
	}

	void IMGuiViewer::operator()()
	{
		if (openGL.NewFrame())
			StopSimulation();
		cam.LookAt(Vec3d(sin(GetOffset().x), 0, -cos(GetOffset().x)), Vec3d(0, 0, 0));
		// Order dependent
		// Process GUI, then render Units, THEN render GUI.
		// So GUI is rendered over the Units, but processed before them to be able to set correct scaleFactor, offset
		imguiBackend.NewFrame(GetFrameTime());
		GUIDrawer->Draw(*data, *this, *lineTrailsDrawer, w, h);
		simDataDrawer->Draw(*data);
		lineTrailsDrawer->Draw(*data);
		imguiBackend.Render();
	}
}
