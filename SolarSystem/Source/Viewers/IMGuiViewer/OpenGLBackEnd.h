#ifndef OPENGL_BACKEND_HEADER
#define OPENGL_BACKEND_HEADER

#include <string>

struct GLFWwindow;

namespace solar
{
	//Handles stuff related to OpenGL
	class OpenGLBackEnd
	{
	public:
		//Initializes OpenGL(GLFW & GLEW) and creates GLFWwindow
		static void Init(int width, int height, const std::string& title);
		//Destroys OpenGL context and GLFW window
		static void Destroy();
		static GLFWwindow* GetWin();
	private:
		static void ErrorCallback(int err, const char* description);
		//Creates shaders for rendering of simData
		static void LoadShaders();
		//Creates VAO,VBO,IBO for rendering of simData
		static void CreateBufferObjects();

		static GLFWwindow* win;
		static std::string error;
		static unsigned int shader;
	};
}

#endif