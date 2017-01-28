#ifndef OPENGL_BACKEND_HEADER
#define OPENGL_BACKEND_HEADER

#include <string>


struct GLFWwindow;

namespace solar
{
	//Initializes OpenGL, GLFW and creates a window
	class OpenGLBackend
	{
	public:
		//Initializes OpenGL(GLFW & GLEW) and creates GLFWwindow
		OpenGLBackend(size_t width, size_t height, const std::string& title);
		//Destroys OpenGL context and GLFW window
		~OpenGLBackend();
		GLFWwindow* GetWin();
		//Returns whether window should be closed(Close button pressed for example)
		bool NewFrame();
	private:
		//Callback function to be called on any GLFW error
		static void ErrorCallback(int err, const char* description);
		GLFWwindow* win;
	};
}

#endif