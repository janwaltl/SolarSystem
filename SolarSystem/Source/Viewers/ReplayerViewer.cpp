#include "ReplayerViewer.h"

namespace solar
{
	ReplayerViewer::ReplayerViewer(const std::string& replayFileName, size_t width, size_t height,
								   const std::string& title) :
		OMSAR(1.0, width / double(height), Vec2(0.0, 0.0)), replayFileName(replayFileName),
		openGL(width, height, title), imguiBackend(openGL.GetWin()), w(width), h(height)
	{
	}
	void ReplayerViewer::Prepare()
	{
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(this->AspectRatio());
		replayGUIDrawer = std::make_unique<drawers::ReplayGUIDrawer>(replayFileName);
		lineTrailsDrawer = std::make_unique<drawers::LineTrailsDrawer>(data->size(), this->AspectRatio());

		ResetZoom(*data, 0.8);
	}
	void ReplayerViewer::operator()()
	{
		if (openGL.NewFrame())
			StopSimulation();
		// Order dependent
		// Process GUI, then render Units, THEN render GUI.
		// So GUI is rendered over the Units, but processed before them to be able to set correct scaleFactor, offset
		imguiBackend.NewFrame(GetFrameTime());
		replayGUIDrawer->Draw(*data, *this, *lineTrailsDrawer, w, h);
		simDataDrawer->Draw(*data, ScaleFactor(), GetOffset());
		lineTrailsDrawer->Draw(*data, ScaleFactor(), GetOffset());
		imguiBackend.Render();
	}
}