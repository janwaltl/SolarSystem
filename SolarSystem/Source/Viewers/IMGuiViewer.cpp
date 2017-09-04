#include "IMGuiViewer.h"
#include <iostream>
namespace solar
{
	namespace
	{
		constexpr float smallGridRes = 10.0f;
	}
	IMGuiViewer::IMGuiViewer(size_t width, size_t height, const std::string& title) :
		openGL(width, height, title), imguiBackend(openGL.GetWin()),
		w(width), h(height)
	{}

	void IMGuiViewer::Prepare()
	{
		GUI = std::make_unique<drawers::GUIDrawer>(*data);
		scene = std::make_unique<drawers::SceneDrawer>(*data, w / double(h));
	}

	void IMGuiViewer::operator()()
	{
		if (openGL.NewFrame())
			StopSimulation();
		// Order dependent
		// Process GUI, then render the scene, THEN render GUI.
		// So GUI is rendered over the scene, but processed before, so it can change stuff - i.e camera
		imguiBackend.NewFrame(GetFrameTime());
		GUI->Draw(*data, *this, *scene, w, h);
		scene->Draw(*data);
		imguiBackend.Render();
		openGL.Render();
	}
}
