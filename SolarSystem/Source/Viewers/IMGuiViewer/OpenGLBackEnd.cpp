#include "OpenGLBackEnd.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Source/Common/Exception.h"
#include "Source/Math/Math.h"
#include "OpenGL/Error.h"


namespace solar
{
	namespace
	{
		//Stores error message
		std::string error;
		bool errTrigger = false;
		constexpr int winPosX = 5;
		constexpr int winPosY = 20;
		//Background color
		constexpr Vec4d bgColor(5/255.0, 10/255.0, 10/255.0, 1.0);
	}

	//Following usage of GLFW library is  based on their tutorial
	// at http://www.glfw.org/docs/latest/quick.html .
	OpenGLBackend::OpenGLBackend(size_t width, size_t height, const std::string & title)
	{
		if (glfwInit() == GL_FALSE)
			throw Exception("Cannot initialize GLFW library.");

		glfwSetErrorCallback(ErrorCallback);
		// Uses OpenGL 1.0 to atleast create contex
		// will automatically use the newest available or throw on some error
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//Do not allow rezising
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);//Anti-aliasing
		win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr); // create actual window
		if (win == nullptr) // if that fails
			throw Exception("Unable to create Window, reason: " + error);

		glfwSetWindowPos(win, winPosX, winPosY);
		glfwMakeContextCurrent(win);

		auto tmpVers = glGetString(GL_VERSION);
		if (tmpVers)
		{
			//Format is MAJOR.MINOR.RELEASE
			std::string version = reinterpret_cast<const char*>(tmpVers);
			int major = version[0] - '0';
			if (major < 3)
				throw Exception("Your version of OpenGL is not 3.0+, application will not work in graphics mode."
								"But, can still be run from command-line with disabled viewer(see -help)"
								"Make sure your graphics drivers are up to date. Detected version: " + version);
			//There is slight chance that this application works even on OpenGL2.0+, but not sure.
			//That means really old PC or no drivers anyway...
			//Also GLSL shaders might not work, but that is heavilly driver specific.
		}
		//else Probably cannot even happen, glfw should've thrown by now


		glewExperimental = GL_TRUE; // Can crash without
		if (glewInit() != GLEW_OK) // tries to initialize glew
			throw Exception("GLEW initialization failed.");

		openGL::CheckForError();//glewInit causes INVALID_ENUM for some reason, this clears it.

		glViewport(0, 0, width, height); // sets correct coordinate viewport

		//Turn off VSYNC if possible - makes frameTime inaccurate
		//Still, might be ignored by driver's settings

		//Enable multisampling to make lineTrails nicer.
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);

		glClearColor((GLclampf)bgColor.x, (GLclampf)bgColor.y, (GLclampf)bgColor.z, (GLclampf)bgColor.w);
		glfwSwapInterval(0);
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
		glfwPollEvents();//Get system events, calls registered callbacks
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
