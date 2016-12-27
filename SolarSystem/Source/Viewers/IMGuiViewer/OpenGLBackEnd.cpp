#include "OpenGLBackEnd.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../Exception.h"

namespace solar
{
	std::string OpenGLBackend::error;

	OpenGLBackend::OpenGLBackend(int width, int height, const std::string & title)
	{
		if (glfwInit() == GL_FALSE)
			throw Exception("Cannot initialize GLFW library.");

		glfwSetErrorCallback(ErrorCallback);

		// Uses OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//Do not allow rezising, because it's too much work to make it nice
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr); // creates actual window
		if (win == nullptr) // if that fails
			throw Exception("Unable to create Window,reason: " + error);

		glfwSetWindowPos(win, 5, 20);
		glfwMakeContextCurrent(win);
		glewExperimental = GL_TRUE; // on some machines OpenGL crashes when this is at false

		if (glewInit() != GLEW_OK) // tries to initialize glew
			throw Exception("GLEW initialization failed.");

		glViewport(0, 0, width, height); // sets correct coordinate viewport
		glfwSwapInterval(0);

	}

	OpenGLBackend::~OpenGLBackend()
	{
		glfwDestroyWindow(win);
		glfwTerminate();

		win = nullptr;
	}

	GLFWwindow * OpenGLBackend::GetWin()
	{
		return win;
	}

	void OpenGLBackend::ErrorCallback(int err, const char * description)
	{
		error = std::string(description);
	}

/*
	void OpenGLBackend::DrawData(const simData_t & data, double scaleFactor, const Vec2& offset)
	{
		unitS->Bind();
		size_t i = 0;
		for (const auto& unit : data)
		{
			if (!trailFrameCounter)
				unitTrails[i].Push(unit.pos);

			unitS->SetUniform4f("col", unit.color);
			unitS->SetUniform2f("scale", 1.0f, 1.0f);
			unitS->SetUniform2f("offset", scaleFactor*unit.pos + offset);
			circleB->Draw();
			unitS->SetUniform2f("offset", offset);
			unitS->SetUniform2f("scale", scaleFactor, scaleFactor);
			unitTrails[i++].Draw();
		}
		++trailFrameCounter %= trailResolution;
		unitS->UnBind();
	}
*/

}
