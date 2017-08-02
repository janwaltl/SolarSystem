#include "ReplayerViewer.h"

namespace solar
{
	ReplayerViewer::ReplayerViewer(const std::string& replayFileName, size_t width, size_t height,
								   const std::string& title) :
		replayFileName(replayFileName), openGL(width, height, title), imguiBackend(openGL.GetWin()), w(width), h(height)
	{}
	void ReplayerViewer::Prepare()
	{
		scene = std::make_unique<drawers::SceneDrawer>(*data);
		GUI = std::make_unique<drawers::ReplayGUIDrawer>(replayFileName);

	}
	void ReplayerViewer::operator()()
	{
		if (openGL.NewFrame())
			StopSimulation();
		// Order dependent
		// Process GUI, then render Units, THEN render GUI.
		// So GUI is rendered over the Units, but processed before them to be able to set correct scaleFactor, offset
		imguiBackend.NewFrame(GetFrameTime());
		GUI->Draw(*data, *this, *scene, w, h);
		scene->Draw(*data);
		imguiBackend.Render();
	}
}