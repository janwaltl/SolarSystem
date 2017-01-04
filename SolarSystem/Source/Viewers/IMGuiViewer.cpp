#include "IMGuiViewer.h"

#include <GLFW/glfw3.h>
#include <algorithm>

#include "IMGuiViewer/OpenGLBackEnd.h"

namespace solar
{
	IMGuiViewer::IMGuiViewer(size_t width, size_t height, const std::string& title) :
		openGL(width, height, title), imguiBackend(openGL.GetWin()),
		scaleFactor(1.0), offset(0.0, 0.0), AR(width / (double)height)
	{
	}

	void IMGuiViewer::Prepare()
	{
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(this->GetAspectRatio());
		GUIDrawer = std::make_unique<drawers::GUIDrawer>(this, data);
		lineTrailsDrawer = std::make_unique<drawers::LineTrailsDrawer>(data->size(), this->GetAspectRatio());
		ResetZoom();
		scaleFactor *= 0.8;//To fit data into <-0.8,0.8> initially
	}

	void IMGuiViewer::operator()()
	{
		if (glfwWindowShouldClose(openGL.GetWin()))
			StopSimulation();
		glfwSwapBuffers(openGL.GetWin());
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Order dependent
		// Process GUI, then render Units, THEN render GUI.
		// So GUI is rendered over the Units, but processed before them to be able to set correct scaleFactor, offset
		imguiBackend.NewFrame();
		GUIDrawer->Draw();
		simDataDrawer->Draw(*data, scaleFactor, offset);
		lineTrailsDrawer->Draw(*data, scaleFactor, offset);
		imguiBackend.Render();
	}

	double IMGuiViewer::ScaleFactor()
	{
		return scaleFactor;
	}

	void IMGuiViewer::ScaleFactor(double newFactor)
	{
		scaleFactor = newFactor;
	}

	void IMGuiViewer::Move(const Vec2 newOffset)
	{
		offset = newOffset;
	}

	Vec2 IMGuiViewer::GetOffset()
	{
		return offset;
	}

	void IMGuiViewer::ResetZoom()
	{
		Vec2 max {std::numeric_limits<double>::min(),std::numeric_limits<double>::min()};
		Vec2 min {std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};

		auto find = [&](const Unit& u) {
			if (u.pos.x > max.x)
				max.x = u.pos.x;
			if (u.pos.y > max.y)
				max.y = u.pos.y;
			if (u.pos.x < min.x)
				min.x = u.pos.x;
			if (u.pos.y < min.y)
				min.y = u.pos.y;
		};
		std::for_each(data->begin(), data->end(), find);

		auto maxL = length(max);
		auto minL = length(min);
		scaleFactor = 1.0 / (maxL > minL ? maxL : minL);
	}

	double IMGuiViewer::GetAspectRatio()
	{
		return AR;
	}

	drawers::LineTrailsDrawer * IMGuiViewer::GetTrailsDrawer()
	{
		assert(lineTrailsDrawer.get());
		return lineTrailsDrawer.get();
	}

}
