#ifndef OPENGL_BACKEND_HEADER
#define OPENGL_BACKEND_HEADER

#include <string>


struct GLFWwindow;

namespace solar
{
	//Handles stuff related to OpenGL
	class OpenGLBackend
	{
	public:
		//Initializes OpenGL(GLFW & GLEW) and creates GLFWwindow
		OpenGLBackend(size_t width, size_t height, const std::string& title);
		//Destroys OpenGL context and GLFW window
		~OpenGLBackend();
		GLFWwindow* GetWin();
		//Returns whether window should be closed
		bool NewFrame();
	private:
		static void ErrorCallback(int err, const char* description);
		//Stores error message
		static std::string error;

		GLFWwindow* win;
	};
}

#endif