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

		gridDrawer.biggerGrid = std::make_unique<drawers::GridDrawer>(cam, 100);
		gridDrawer.smallerGrid = std::make_unique<drawers::GridDrawer>(cam, 1000);


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
		auto newCamDist = cam.CamPos().z;
		scale *= 0.2f;
		auto val = log(newCamDist) / log(smallGridRes);
		auto smallerScale = scale  * powf(smallGridRes, floor(val + .0001f));
		auto biggerScale = smallerScale*smallGridRes;
		float x;
		auto frac = abs(modff(val, &x));
		frac = val > 0 ? frac : 1 - frac;
		auto alfa = 1 - frac;
		gridDrawer.smallerGrid->Draw(gridDrawer.smallerGrid->XY, smallerScale, Vec4f(0.96f*frac + 0.06f*alfa, 0.24f, 0.10f*frac + 0.71f*alfa, 1.0f*alfa), frac);
		gridDrawer.biggerGrid->Draw(gridDrawer.biggerGrid->XY, biggerScale, Vec4f(0.06f, 0.24f, 0.71f, 1.00f), frac);
	}
	Camera & IMGuiViewer::GetCamera()
	{
		return cam;
	}
}
