#include "IMGuiViewer.h"

#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/IMGuiLibrary/imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>

namespace solar
{
	IMGuiViewer::IMGuiViewer()
	{
		OpenGLBackEnd::Init(640, 480, "Simulation");
		ImGui_ImplGlfwGL3_Init(OpenGLBackEnd::GetWin(), true);
	}

	IMGuiViewer::~IMGuiViewer()
	{
		ImGui_ImplGlfwGL3_Shutdown();
		OpenGLBackEnd::Destroy();
	}

	void IMGuiViewer::operator()(simData_t & data)
	{
		if (glfwWindowShouldClose(OpenGLBackEnd::GetWin()))
			StopSimulation();
		glfwPollEvents();
		glfwSwapBuffers(OpenGLBackEnd::GetWin());
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render Data

		ImGui_ImplGlfwGL3_NewFrame();
		//Render GUI
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Always);
		ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Testing text.");
		ImGui::End();
		ImGui::Render();
	}

}
