#include "IMGuiViewer.h"

#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/IMGuiLibrary/imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace solar
{
	IMGuiViewer::IMGuiViewer():
		openGL(640, 640, "Simulation"),scaleFactor(1.0)
	{
		ImGui_ImplGlfwGL3_Init(openGL.GetWin(), true);
	}

	IMGuiViewer::~IMGuiViewer()
	{
		ImGui_ImplGlfwGL3_Shutdown();
	}

	void IMGuiViewer::operator()(simData_t & data)
	{
		if (glfwWindowShouldClose(openGL.GetWin()))
			StopSimulation();
		glfwSwapBuffers(openGL.GetWin());
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render Data
		openGL.DrawData(data,scaleFactor*0.8);//To fit data into <-0.8,0.8>
		ImGui_ImplGlfwGL3_NewFrame();

		//Render GUI
		{
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Always);
			ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Testing text.");
			ImGui::End();
		}
		ImGui::Render();
	}

	void IMGuiViewer::Prepare(const simData_t & data)
	{
		openGL.CreateBufferObjects(data.size());
		scaleFactor = NormalizeData(data);
	}

	double IMGuiViewer::NormalizeData(const simData_t & data)
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
		std::for_each(data.begin(), data.end(), find);

		auto maxL = length(max);
		auto minL = length(min);
		return 1.0 / ( maxL>minL ? maxL : minL);
	}

}
