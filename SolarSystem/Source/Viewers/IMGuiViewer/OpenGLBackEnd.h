#ifndef OPENGL_BACKEND_HEADER
#define OPENGL_BACKEND_HEADER

#include <string>
#include <memory>

#include "Source/Common/Common.h"

struct GLFWwindow;

namespace solar
{
	namespace openGLBackend
	{
		class Shader;
		class CircleBuffer;
		class UnitTrail;
	}
	//Handles stuff related to OpenGL
	class OpenGLBackend
	{
	public:
		//Initializes OpenGL(GLFW & GLEW) and creates GLFWwindow
		OpenGLBackend(int width, int height, const std::string& title);
		//Destroys OpenGL context and GLFW window
		~OpenGLBackend();
		GLFWwindow* GetWin();
	private:
		static void ErrorCallback(int err, const char* description);
		//Stores error message
		static std::string error;

		GLFWwindow* win;
	};
}

#endif