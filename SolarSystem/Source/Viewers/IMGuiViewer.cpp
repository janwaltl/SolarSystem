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
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(cam);
		GUIDrawer = std::make_unique<drawers::GUIDrawer>();
		lineTrailsDrawer = std::make_unique<drawers::LineTrailsDrawer>(data->Get().size(), cam);
		//testDrawer = std::make_unique<drawers::TestDrawer>(cam);

		gridDrawer = std::make_unique<drawers::GridDrawer>(*data, cam, 100, 10);

		//cam.MakeOrtho(17*2,10*2, 0.1f, 10000.0f);
		cam.MakePerspective(90, 1.7f, 0.01f, 10000.0f);
		cam.LookAt(Vec3d(0, 0, 1.0), Vec3d(0, 0, 0));
	}

	void IMGuiViewer::operator()()
	{
		if (openGL.NewFrame())
			StopSimulation();
		// Order dependent
		// Process GUI, then render Units, THEN render GUI.
		// So GUI is rendered over the Units, but processed before them to be able to set correct scaleFactor, offset
		imguiBackend.NewFrame(GetFrameTime());
		GUIDrawer->Draw(*data, *this, *lineTrailsDrawer, w, h);
		lineTrailsDrawer->Draw(*data);
		DrawGrid();
		simDataDrawer->Draw(*data);
		//testDrawer->Draw();
		imguiBackend.Render();
	}
	void IMGuiViewer::DrawGrid()
	{
		Vec2f scale(data->RatioOfDistTo(PhysUnits::AU), data->RatioOfDistTo(PhysUnits::AU));
		gridDrawer->Draw(*data,cam, gridDrawer->YZ, scale, -0.1f);
	}
	Camera & IMGuiViewer::GetCamera()
	{
		return cam;
	}
}
