#include "IMGuiViewer.h"

#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/IMGuiLibrary/imgui_impl_glfw_gl3.h"

namespace solar
{
	IMGuiViewer::IMGuiViewer()
	{
		opengl::Init(640,48,"Simulation");
		ImGui_ImplGlfwGL3_Init(opengl::win, false);
	}

	IMGuiViewer::~IMGuiViewer()
	{
		ImGui_ImplGlfwGL3_Shutdown();
		opengl::Destroy();
	}

	void IMGuiViewer::operator()(simData_t & data)
	{
		if (glfwWindowShouldClose(opengl::win))
			StopSimulation();
		glfwPollEvents();
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Render Data

		ImGui_ImplGlfwGL3_NewFrame();
		//Render GUI
		ImGui::Render();
		glfwSwapBuffers(opengl::win);
	}

}
