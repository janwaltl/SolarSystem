#include "OpenGLBackEnd.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Source/Common/Exception.h"
#include "OpenGL/Error.h"

namespace solar
{
	namespace
	{
		//Stores error message
		std::string error;
		bool errTrigger = false;
		constexpr int winPosX = 5;
		constexpr int winPosY = 5;
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
		//Do not allow rezising, because it's too much work to make it nice
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr); // create actual window
		if (win == nullptr) // if that fails
			throw Exception("Unable to create Window,reason: " + error);

		glfwSetWindowPos(win, winPosX, winPosY);
		glfwMakeContextCurrent(win);
		glewExperimental = GL_TRUE; // Can crash without

		if (glewInit() != GLEW_OK) // tries to initialize glew
			throw Exception("GLEW initialization failed.");

		openGL::CheckForError();//glewInit causes INVALID_ENUM for some reason...

		glViewport(0, 0, width, height); // sets correct coordinate viewport
		glfwSwapInterval(0);//turn off VSYNC if possible - to prevent useless slow-down

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
		glfwPollEvents();
		glClearColor(0.0, 0.0, 0.0, 1.0);
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
