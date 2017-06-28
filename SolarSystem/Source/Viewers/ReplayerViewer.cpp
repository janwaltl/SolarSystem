#include "ReplayerViewer.h"

namespace solar
{
	ReplayerViewer::ReplayerViewer(const std::string& replayFileName, size_t width, size_t height,
								   const std::string& title) :
		replayFileName(replayFileName), openGL(width, height, title), imguiBackend(openGL.GetWin()), w(width), h(height)
	{}
	void ReplayerViewer::Prepare()
	{
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(cam);
		replayGUIDrawer = std::make_unique<drawers::ReplayGUIDrawer>(replayFileName);
		lineTrailsDrawer = std::make_unique<drawers::LineTrailsDrawer>(data->Get().size(), cam);

		//cam.MakeOrtho(17*2,10*2, 0.1f, 10000.0f);
		cam.MakePerspective(90, 1.7f, 0.1f, 10000.0f);
		cam.LookAt(Vec3d(0, 0, 1.0), Vec3d(0, 0, 0));
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
		simDataDrawer->Draw(*data);
		lineTrailsDrawer->Draw(*data);
		imguiBackend.Render();
	}
	Camera & ReplayerViewer::GetCamera()
	{
		return cam;
	}
}