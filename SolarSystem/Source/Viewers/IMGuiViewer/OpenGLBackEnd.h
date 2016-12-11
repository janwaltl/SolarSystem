#ifndef OPENGL_BACKEND_HEADER
#define OPENGL_BACKEND_HEADER

#include <string>
#include <memory>
#include "../../Common/Common.h"

struct GLFWwindow;
namespace solar
{
	namespace openGLBackend
	{
		class Shader;
		class CircleBuffer;
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
		// Creates VAOs for Units(CircleBuffer) and line trails
		void CreateBufferObjects(size_t numUnits);
		// Draws simulated data
		void DrawData(const simData_t& data, double scaleFactor);
	private:
		static void ErrorCallback(int err, const char* description);

		//Creates VAO,VBO,IBO for rendering of simData
		
		void CreateShaders();
		//Hard-coded shaders
		std::string GetUnitVertSource();
		std::string GetUnitFragSource();

		GLFWwindow* win;
		static std::string error;//Stores error message

		std::unique_ptr<openGLBackend::Shader> unitS;
		//Used for drawing Units
		std::unique_ptr<openGLBackend::CircleBuffer> circleB;

	};
}

#endif