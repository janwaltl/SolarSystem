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
		void CreateFBO(const size_t &width, const size_t &height);
		//Destroys OpenGL context and GLFW window
		~OpenGLBackend();
		GLFWwindow* GetWin();
		//Returns whether window should be closed(Close button pressed for example)
		//Bind framebuffer into which should everything be rendered
		bool NewFrame();
		//Copies contents of FBO to screen
		void Render();
	private:
		//Callback function to be called on any GLFW error
		static void ErrorCallback(int err, const char* description);

		void CheckVersionAndExtensions();
		unsigned int FBO, FBOColTex, FBODepthTex;
		GLFWwindow* win;
	};
}

#endif