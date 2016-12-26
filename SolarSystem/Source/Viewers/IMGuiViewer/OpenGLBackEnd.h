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
		class UnitTrail;
	}
	//Handles stuff related to OpenGL
	class OpenGLBackend
	{
	public:
		//Initializes OpenGL(GLFW & GLEW) and creates GLFWwindow
		OpenGLBackend(int width, int height, const std::string& title, float circleSize, size_t circleResolution);
		//Destroys OpenGL context and GLFW window
		~OpenGLBackend();
		GLFWwindow* GetWin();
		// Creates VAOs for Units(CircleBuffer) and line trails
		void CreateBufferObjects(size_t numUnits);
		// Draws simulated data
		void DrawData(const simData_t& data, double scaleFactor, const Vec2& offset);
	private:
		static void ErrorCallback(int err, const char* description);

		void CreateShaders();
		//Hard-coded shaders
		std::string GetUnitVertSource();
		std::string GetUnitFragSource();

		//Stores error message
		static std::string error;
		//Trail will get updated(newPos pushed in) each n frames
		const static size_t trailResolution = 10;

		GLFWwindow* win;
		std::unique_ptr<openGLBackend::Shader> unitS;
		//Used for drawing Units
		std::unique_ptr<openGLBackend::CircleBuffer> circleB;
		std::vector<openGLBackend::UnitTrail> unitTrails;
		//Counts frames between trails' update
		size_t trailFrameCounter;
		double aspectRatio;
		float cSize;
		size_t cResolution;
	};
}

#endif