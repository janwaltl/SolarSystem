#ifndef OPENGL_BACKEND_HEADER
#define OPENGL_BACKEND_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../Exception.h"

namespace solar
{
	namespace opengl
	{
		static GLFWwindow* win = nullptr;
		static std::string error;

		void error_callback(int err, const char* description)
		{
			error=std::string(description);
		}

		void Init(int width, int height, const std::string& title)
		{
			if (glfwInit() == GL_FALSE)
				throw Exception("Cannot initialize GLFW library.");

			glfwSetErrorCallback(error_callback);

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

		}
		void Destroy()
		{
			glfwDestroyWindow(win);
			glfwTerminate();
			win = nullptr;
		}
	}
}

#endif