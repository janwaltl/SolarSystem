#include "IMGuiViewer.h"

#include <GLFW/glfw3.h>
#include <algorithm>

#include "IMGuiViewer/OpenGLBackEnd.h"

namespace solar
{
	IMGuiViewer::IMGuiViewer(int width, int height, const std::string& title /*= "Simulation"*/) :
		openGL(width, height, title), imguiBackend(openGL.GetWin()),
		scaleFactor(1.0), offset(0.0, 0.0), AR(width / (double)height), gui(this)
	{
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
		gui.Draw();
		simDataDrawer->Draw();
		imguiBackend.Render();
	}

	void IMGuiViewer::Prepare()
	{
		gui.Prepare(data);
		simDataDrawer = std::make_unique<drawers::SimDataDrawer>(this, data);

		ResetZoom();
		scaleFactor *= 0.8;//To fit data into <-0.8,0.8> initially
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
			if (u.pos.X() > max.X())
				max.X(u.pos.X());
			if (u.pos.Y() > max.Y())
				max.Y(u.pos.Y());
			if (u.pos.X() < min.X())
				min.X(u.pos.X());
			if (u.pos.Y() < min.Y())
				min.Y(u.pos.Y());
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

}
