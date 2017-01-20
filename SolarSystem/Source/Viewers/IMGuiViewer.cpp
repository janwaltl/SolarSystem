#include "IMGuiViewer.h"

namespace solar
{
	IMGuiViewer::IMGuiViewer(size_t width, size_t height, const std::string& title) :
		OMSAR(1.0, width / double(height), Vec2(0.0, 0.0)),
		openGL(width, height, title), imguiBackend(openGL.GetWin())
	{
	}

	void IMGuiViewer::Prepare()
	{
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(this->AspectRatio());
		GUIDrawer = std::make_unique<drawers::GUIDrawer>(this, data);
		lineTrailsDrawer = std::make_unique<drawers::LineTrailsDrawer>(data->size(), this->AspectRatio());

		ResetZoom(*data, 0.8);
	}

	void IMGuiViewer::operator()()
	{
		if (openGL.NewFrame())
			StopSimulation();
		// Order dependent
		// Process GUI, then render Units, THEN render GUI.
		// So GUI is rendered over the Units, but processed before them to be able to set correct scaleFactor, offset
		imguiBackend.NewFrame();
		GUIDrawer->Draw();
		simDataDrawer->Draw(*data, ScaleFactor(), GetOffset());
		lineTrailsDrawer->Draw(*data, ScaleFactor(), GetOffset());
		imguiBackend.Render();
	}

	drawers::LineTrailsDrawer * IMGuiViewer::GetTrailsDrawer()
	{
		assert(lineTrailsDrawer.get());
		return lineTrailsDrawer.get();
	}

}
