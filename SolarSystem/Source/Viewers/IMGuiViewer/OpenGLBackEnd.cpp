#include "OpenGLBackEnd.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Source/Common/Exception.h"
#include "Source/Math/Math.h"
#include "OpenGL/Error.h"

namespace solar
{
	namespace
	{
		//Stores error message
		std::string error;
		bool errTrigger = false;
		constexpr int winPosX = 5;
		constexpr int winPosY = 20;
		//Background color
		constexpr Vec4 bgColor(0.0, 0.0, 0.0, 1.0);
	}

	//Following usage of GLFW library is  based on their tutorial
	// at http://www.glfw.org/docs/latest/quick.html .
	OpenGLBackend::OpenGLBackend(size_t width, size_t height, const std::string & title)
	{
		if (glfwInit() == GL_FALSE)
			throw Exception("Cannot initialize GLFW library.");

		glfwSetErrorCallback(ErrorCallback);

		// Uses OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//Do not allow rezising
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);//Anti-aliasing
		win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr); // create actual window
		if (win == nullptr) // if that fails
			throw Exception("Unable to create Window,reason: " + error);

		glfwSetWindowPos(win, winPosX, winPosY);
		glfwMakeContextCurrent(win);
		glewExperimental = GL_TRUE; // Can crash without

		if (glewInit() != GLEW_OK) // tries to initialize glew
			throw Exception("GLEW initialization failed.");

		openGL::CheckForError();//glewInit causes INVALID_ENUM for some reason, this clears it.

		glViewport(0, 0, width, height); // sets correct coordinate viewport
		//Turn off VSYNC if possible - makes frameTime inaccurate
		//Still, might be ingnored by driver's settings
		glEnable(GL_MULTISAMPLE);
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

	bool OpenGLBackend::NewFrame()
	{
		glfwSwapBuffers(win);
		glfwPollEvents();//Get system events, calls registered callbacks
		glClearColor((GLclampf)bgColor.x, (GLclampf)bgColor.y, (GLclampf)bgColor.z, (GLclampf)bgColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		if (errTrigger)
		{
			errTrigger = false;
			throw Exception(error);
		}
		return glfwWindowShouldClose(win) != 0;
	}

	void OpenGLBackend::ErrorCallback(int err, const char * description)
	{
		error = "GLFW error: " + std::to_string(err) + ": " + description;
		errTrigger = true;
	}
}
