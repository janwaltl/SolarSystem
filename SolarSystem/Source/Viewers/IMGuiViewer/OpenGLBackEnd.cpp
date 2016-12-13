#include "OpenGLBackEnd.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../Exception.h"
#include "OpenGLBackend/Shader.h"
#include "OpenGLBackend/CircleBuffer.h"

namespace solar
{
	std::string OpenGLBackend::error;

	OpenGLBackend::OpenGLBackend(int width, int height, const std::string & title, float circleSize,
								 size_t circleResolution) :
		unitS(nullptr)
	{
		aspectRatio = double(width) / height;
		cSize = circleSize;
		cResolution = circleResolution;
		if (glfwInit() == GL_FALSE)
			throw Exception("Cannot initialize GLFW library.");

		glfwSetErrorCallback(ErrorCallback);

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
		glfwSwapInterval(0);

		CreateShaders();///IF IT Throws, glfw needs to be destroyed
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

	void OpenGLBackend::ErrorCallback(int err, const char * description)
	{
		error = std::string(description);
	}


	void OpenGLBackend::CreateBufferObjects(size_t numUnits)
	{
		circleB = std::make_unique<openGLBackend::CircleBuffer>(cResolution, cSize);
	}

	void OpenGLBackend::DrawData(const simData_t & data, double scaleFactor)
	{
		unitS->Bind();
		for (const auto& unit : data)
		{
			unitS->SetUniform4f("col", unit.color);
			unitS->SetUniform2f("offset", scaleFactor*unit.pos);
			circleB->Draw();
		}
		unitS->UnBind();
	}

	void OpenGLBackend::CreateShaders()
	{
		unitS = std::make_unique<openGLBackend::Shader>(GetUnitVertSource(), GetUnitFragSource());
		unitS->SetUniform2f("AR", {aspectRatio,1.0});
	}


	std::string OpenGLBackend::GetUnitVertSource()
	{
		return R"(
			#version 330 core
			layout(location = 0) in vec2 position;

			uniform vec2 offset;
			uniform vec2 AR;
			void main()
			{
				gl_Position = vec4(position.x + offset.x,AR.x*( position.y + offset.y), 0.0, 1.0);
			})";
	}

	std::string OpenGLBackend::GetUnitFragSource()
	{
		return R"(
			#version 330 core
			out vec4 color;

			uniform vec4 col;

			void main()
			{
				color = col;
			})";
	}

}
